#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <mutex>

std::mutex mtx;

void sumarNumeros(int id, std::vector<int>& resultados) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 1000);

    int suma = 0;
    for (int i = 0; i < 100; i++) {
        suma += dist(gen);
    }

    {
        std::lock_guard<std::mutex> lock(mtx);
        resultados[id] = suma;
        std::cout << "Thread " << id << " obtuvo suma = " << suma << std::endl;
    }
}

int main() {
    const int num_threads = 10;
    std::vector<std::thread> threads;
    std::vector<int> resultados(num_threads, 0);

// Crear hilos
    for (int i = 0; i < num_threads; i++) {
        threads.emplace_back(sumarNumeros, i, std::ref(resultados));
    }

// Esperar a que terminen
    for (auto& t : threads) {
        t.join();
    }

// Buscar el mayor
    int maxId = 0;
    for (int i = 1; i < num_threads; i++) {
        if (resultados[i] > resultados[maxId]) {
            maxId = i;
        }
    }

    std::cout << "El thread ganador es el " << maxId << " con la suma más alta = " << resultados[maxId] << std::endl;

    return 0;
}
