#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <random>
#include <cstddef>

using namespace std;
using namespace std::chrono;

using TI = int;
using TD = double;

// Parámetros de la simulación
const int N = 1000;              // Tamaño de la matriz (N x N)
const int NUM_THREADS = 6;       // Número de hilos a utilizar

// Función clásica de un solo hilo
template <typename T>
void multiplySingleThread(const vector<T>& A, const vector<T>& B, vector<T>& C) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            TD sum = 0.0;
            for (size_t k = 0; k < N; ++k) {
                // Aplanamiento de índices 2D a 1D: fila * N + columna
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

// Función "Worker" que ejecutará cada hilo
// Cada hilo procesa solo un bloque de filas de la matriz resultante
template <typename T>
void multiplyWorker(size_t startRow, size_t endRow, const vector<T>& A, const vector<T>& B, vector<T>& C) {
    for (size_t i = startRow; i < endRow; ++i) {
        for (size_t j = 0; j < N; ++j) {
            TD sum = 0.0;
            for (size_t k = 0; k < N; ++k) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

// Función gestora para la ejecución multihilo
template <typename T>
void multiplyMultiThread(const vector<T>& A, const vector<T>& B, vector<T>& C) {
    vector<thread> threads;
    TI rowsPerThread = N / NUM_THREADS;

    // Lanzar los hilos
    for (size_t i = 0; i < NUM_THREADS; ++i) {
        size_t startRow = i * rowsPerThread;
        // El último hilo toma cualquier fila restante si N no es divisible exactamente
        size_t endRow = (i == NUM_THREADS - 1) ? N : startRow + rowsPerThread;
        
        threads.emplace_back(multiplyWorker<T>, startRow, endRow, cref(A), cref(B), ref(C));
    }

    // Esperar a que todos los hilos terminen su trabajo
    for (auto& t : threads) {
        t.join();
    }
}

int main() {
    cout << "Inicializando matrices de " << N << "x" << N << "..." << endl;
    
    // Asignación de memoria en el Heap (Heap allocation)
    vector<double> A(N * N);
    vector<double> B(N * N);
    vector<double> C_single(N * N, 0.0);
    vector<double> C_multi(N * N, 0.0);

    // Llenar con números aleatorios
    mt19937 rng(42);
    uniform_real_distribution<double> dist(1.0, 10.0);
    for (int i = 0; i < N * N; ++i) {
        A[i] = dist(rng);
        B[i] = dist(rng);
    }

    // --- UN SOLO HILO ---
    cout << "Ejecutando multiplicacion en 1 Hilo..." << endl;
    auto startSingle = high_resolution_clock::now();
    
    multiplySingleThread(A, B, C_single);
    
    auto endSingle = high_resolution_clock::now();
    duration<double> diffSingle = endSingle - startSingle;
    cout << "Tiempo 1 Hilo: " << diffSingle.count() << " segundos.\n" << endl;

    // --- MULTIHILO ---
    cout << "Ejecutando multiplicacion en " << NUM_THREADS << " Hilos..." << endl;
    auto startMulti = high_resolution_clock::now();
    
    multiplyMultiThread(A, B, C_multi);
    
    auto endMulti = high_resolution_clock::now();
    duration<double> diffMulti = endMulti - startMulti;
    cout << "Tiempo " << NUM_THREADS << " Hilos: " << diffMulti.count() << " segundos.\n" << endl;

    // Mostrar el factor de aceleración (Speedup)
    cout << "======================================" << endl;
    cout << "Aceleracion (Speedup): " << diffSingle.count() / diffMulti.count() << "x" << endl;
    
    return 0;
}