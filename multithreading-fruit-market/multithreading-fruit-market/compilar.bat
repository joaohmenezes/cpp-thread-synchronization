@echo off
echo Compilando o sistema...
g++ -std=c++11 main.cpp -o sistema_frutas.exe -pthread
if %errorlevel% equ 0 (
    echo Compilacao concluida com sucesso!
    echo Execute: sistema_frutas.exe
) else (
    echo Erro na compilacao!
)
pause

