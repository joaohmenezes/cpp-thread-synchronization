# cpp-thread-synchronization
C++ multithreading simulation of a supply chain. Solves a complex Producer-Consumer problem using mutexes and condition variables to synchronize threads dynamically.

# Thread Synchronization: Supply Chain Simulation

## About the Project
This project is a **C++ Multithreading Simulation** that models a complete fruit supply chain, from production to final sale. It is a complex variation of the classic **Producer-Consumer problem**, requiring strict thread synchronization and resource management.

**Academic Context:** Practical Project #2 developed for the Computer Engineering course at the Federal University of Itajubá (UNIFEI) - Campus Itabira.

## Architecture and Concurrency
The system simulates concurrent tasks performed by different entities, applying mutual exclusion mechanisms to synchronize access to shared resources.

* **Producers (Farmers):** Tiago, João, and Alice produce watermelons, grapes, and strawberries, storing them in a central barn.
* **Transporters:** Pedro and Bino load the fruits from the barn and transport them to the Kategatt city market.
* **Consumers (Clients):** Multiple clients buy the fruits from the market manager (Ragnar).

### Technical Features
* **Thread Management:** Uses the `<thread>` library to manage independent actors concurrently.
* **Mutual Exclusion & Locks:** Utilizes `std::mutex`, `std::unique_lock`, and `std::condition_variable` to prevent race conditions when accessing the Barn and the Market.
* **Dynamic Configuration:** The system allows real-time modifications to the execution times (production, transportation, and purchase times) to simulate and analyze different bottleneck scenarios during runtime.
* **Custom Capacities:** Takes the maximum number of clients and the storage capacities of both the barn and the market as initial inputs.

## Technologies Used
* **C++11 or higher** (Required for standard thread library)
