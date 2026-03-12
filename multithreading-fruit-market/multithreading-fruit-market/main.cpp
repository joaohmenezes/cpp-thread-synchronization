#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <string>
#include <chrono>
#include <atomic>
#include <map>

using namespace std;

struct Config {
    int capacidade_celeiro;
    int capacidade_caminhao;
    int capacidade_mercado;
    int num_clientes;
    
    atomic<int> tempo_producao;
    atomic<int> tempo_transporte;
    atomic<int> tempo_compra;
};

Config config;
atomic<bool> sistema_ativo(true);

class Celeiro {
public:
    Celeiro(int capacidade) : cap(capacidade), total(0) {
        frutas["melancia"] = 0;
        frutas["uva"] = 0;
        frutas["morango"] = 0;
    }
    
    bool adicionar(string fruta) {
        unique_lock<mutex> lock(mtx);
        cv_espaco.wait(lock, [this] { return total < cap || !sistema_ativo; });
        
        if (!sistema_ativo) return false;
        
        frutas[fruta]++;
        total++;
        cout << "[CELEIRO] " << fruta << " armazenada. Total: " << total 
             << "/" << cap << endl;
        
        cv_item.notify_all();
        return true;
    }
    
    bool remover(string fruta, int quantidade) {
        unique_lock<mutex> lock(mtx);
        cv_item.wait(lock, [this, fruta, quantidade] { 
            return frutas[fruta] >= quantidade || !sistema_ativo; 
        });
        
        if (!sistema_ativo) return false;
        
        frutas[fruta] -= quantidade;
        total -= quantidade;
        
        cv_espaco.notify_all();
        return true;
    }
    
    int get_quantidade(string fruta) {
        lock_guard<mutex> lock(mtx);
        return frutas[fruta];
    }

private:
    int cap;
    int total;
    map<string, int> frutas;
    mutex mtx;
    condition_variable cv_espaco;
    condition_variable cv_item;
};

class Mercado {
public:
    Mercado(int capacidade) : cap(capacidade), total(0) {
        frutas["melancia"] = 0;
        frutas["uva"] = 0;
        frutas["morango"] = 0;
    }
    
    bool adicionar(string fruta, int quantidade) {
        unique_lock<mutex> lock(mtx);
        cv_espaco.wait(lock, [this, quantidade] { 
            return (total + quantidade) <= cap || !sistema_ativo; 
        });
        
        if (!sistema_ativo) return false;
        
        frutas[fruta] += quantidade;
        total += quantidade;
        cout << "[MERCADO] " << quantidade << " " << fruta << "(s) entregues. Total: " 
             << total << "/" << cap << endl;
        
        cv_item.notify_all();
        return true;
    }
    
    bool comprar(string fruta) {
        unique_lock<mutex> lock(mtx);
        cv_item.wait(lock, [this, fruta] { 
            return frutas[fruta] > 0 || !sistema_ativo; 
        });
        
        if (!sistema_ativo) return false;
        
        frutas[fruta]--;
        total--;
        
        cv_espaco.notify_all();
        return true;
    }
    
    string escolher_fruta() {
        lock_guard<mutex> lock(mtx);
        for (auto& p : frutas) {
            if (p.second > 0) return p.first;
        }
        return "";
    }

private:
    int cap;
    int total;
    map<string, int> frutas;
    mutex mtx;
    condition_variable cv_espaco;
    condition_variable cv_item;
};

Celeiro* celeiro;
Mercado* mercado;

void fazendeiro(string nome, string fruta) {
    while (sistema_ativo) {
        this_thread::sleep_for(chrono::milliseconds(config.tempo_producao));
        cout << nome << " colheu " << fruta << endl;
        
        if (!celeiro->adicionar(fruta)) break;
    }
}

void transportador(string nome) {
    vector<string> tipos = {"melancia", "uva", "morango"};
    
    while (sistema_ativo) {
        int carga = config.capacidade_caminhao;
        map<string, int> carregamento;
        
        for (auto& fruta : tipos) {
            int pegar = min(carga / 3, 2);
            if (pegar > 0) {
                if (!celeiro->remover(fruta, pegar)) return;
                carregamento[fruta] = pegar;
                carga -= pegar;
            }
        }
        
        cout << "[TRANSPORTE] " << nome << " saiu para Kategatt" << endl;
        this_thread::sleep_for(chrono::milliseconds(config.tempo_transporte));
        cout << "[TRANSPORTE] " << nome << " chegou em Kategatt" << endl;
        
        for (auto& p : carregamento) {
            if (!mercado->adicionar(p.first, p.second)) return;
        }
    }
}

void cliente(int id) {
    while (sistema_ativo) {
        this_thread::sleep_for(chrono::milliseconds(config.tempo_compra));
        
        string fruta = mercado->escolher_fruta();
        if (fruta.empty()) {
            this_thread::sleep_for(chrono::milliseconds(500));
            continue;
        }
        
        if (!mercado->comprar(fruta)) break;
        cout << "[CLIENTE " << id << "] comprou " << fruta << endl;
        
        this_thread::sleep_for(chrono::milliseconds(config.tempo_compra * 2));
    }
}

void monitor_sistema() {
    cout << "\nComandos" << endl;
    cout << "1 - Alterar tempo de producao" << endl;
    cout << "2 - Alterar tempo de transporte" << endl;
    cout << "3 - Alterar tempo de compra" << endl;
    cout << "0 - Encerrar sistema" << endl;
    cout << "\n" << endl;
    
    while (sistema_ativo) {
        int cmd;
        cin >> cmd;
        
        if (cmd == 0) {
            sistema_ativo = false;
            cout << "Encerrando sistema..." << endl;
        } else if (cmd == 1) {
            int novo;
            cout << "Novo tempo de producao (ms): ";
            cin >> novo;
            config.tempo_producao = novo;
            cout << "Tempo atualizado" << endl;
        } else if (cmd == 2) {
            int novo;
            cout << "Novo tempo de transporte (ms): ";
            cin >> novo;
            config.tempo_transporte = novo;
            cout << "Tempo atualizado!" << endl;
        } else if (cmd == 3) {
            int novo;
            cout << "Novo tempo de compra (ms): ";
            cin >> novo;
            config.tempo_compra = novo;
            cout << "Tempo atualizado!" << endl;
        }
    }
}

int main() {
    cout << "   Mercadinho da UNIFEI    " << endl;
    
    cout << "Capacidade do celeiro: ";
    cin >> config.capacidade_celeiro;
    
    cout << "Capacidade do caminhao: ";
    cin >> config.capacidade_caminhao;
    
    cout << "Capacidade do mercado: ";
    cin >> config.capacidade_mercado;
    
    cout << "Numero de clientes: ";
    cin >> config.num_clientes;
    
    cout << "Tempo de producao (ms): ";
    int t;
    cin >> t;
    config.tempo_producao = t;
    
    cout << "Tempo de transporte (ms): ";
    cin >> t;
    config.tempo_transporte = t;
    
    cout << "Tempo de compra (ms): ";
    cin >> t;
    config.tempo_compra = t;
    
    celeiro = new Celeiro(config.capacidade_celeiro);
    mercado = new Mercado(config.capacidade_mercado);
    
    vector<thread> threads;
    
    threads.push_back(thread(fazendeiro, "Tiago", "melancia"));
    threads.push_back(thread(fazendeiro, "Joao", "uva"));
    threads.push_back(thread(fazendeiro, "Alice", "morango"));
    
    threads.push_back(thread(transportador, "Pedro"));
    threads.push_back(thread(transportador, "Bino"));
    
    for (int i = 1; i <= config.num_clientes; i++) {
        threads.push_back(thread(cliente, i));
    }
    
    thread monitor(monitor_sistema);
    
    monitor.join();
    
    for (auto& t : threads) {
        t.join();
    }
    
    delete celeiro;
    delete mercado;
    
    cout << "!!! Sistema finalizado !!!" << endl;
    
    return 0;
}

