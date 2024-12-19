#include <CL/sycl.hpp>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;
using namespace sycl;

// Función secuencial de multiplicación de matrices
void matrix_multiply_sequential(const std::vector<float>& A, const std::vector<float>& B, 
                                std::vector<float>& C, size_t N) {
    for (size_t i = 0; i < N; i++) {
        for (size_t j = 0; j < N; j++) {
            float sum = 0;
            for (size_t k = 0; k < N; k++) {
                sum += A[i * N + k] * B[k * N + j];
            }
            C[i * N + j] = sum;
        }
    }
}

// Función paralela de multiplicación de matrices con SYCL
void matrix_multiply_parallel(const std::vector<float>& A, const std::vector<float>& B, 
                              std::vector<float>& C, size_t N, queue& q) {
    buffer a_buf(A.data(), range<1>(N * N));
    buffer b_buf(B.data(), range<1>(N * N));
    buffer c_buf(C.data(), range<1>(N * N));

    q.submit([&](handler& h) {
        auto a = a_buf.get_access<access::mode::read>(h);
        auto b = b_buf.get_access<access::mode::read>(h);
        auto c = c_buf.get_access<access::mode::write>(h);

        h.parallel_for(range<2>(N, N), [=](id<2> idx) {
            size_t row = idx[0];
            size_t col = idx[1];
            float sum = 0;

            for (size_t k = 0; k < N; k++) {
                sum += a[row * N + k] * b[k * N + col];
            }
            c[row * N + col] = sum;
        });
    });

    q.wait(); // Sincroniza las operaciones
}

// Función para imprimir matrices
void print_matrix(const std::vector<float>& matrix, size_t N) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            std::cout << matrix[i * N + j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    const size_t N = 1024; // Tamaño de la matriz (NxN)
    std::vector<float> A(N * N, 1.0); // Inicializa con 1s
    std::vector<float> B(N * N, 2.0); // Inicializa con 2s
    std::vector<float> C(N * N, 0.0); // Resultado

    // Crear una cola SYCL para la CPU
    queue cpu_queue{sycl::cpu_selector_v};
    std::cout << "\nUsando dispositivo (CPU): " << cpu_queue.get_device().get_info<info::device::name>() << "\n";

    // Crear una cola SYCL para la GPU
    queue gpu_queue{sycl::gpu_selector_v};
    std::cout << "Usando dispositivo (GPU): " << gpu_queue.get_device().get_info<info::device::name>() << "\n\n";

    // ========================================
    // Multiplicación secuencial (en la CPU)
    // ========================================
    auto start = chrono::high_resolution_clock::now();
    matrix_multiply_sequential(A, B, C, N);  // Multiplicación secuencial (CPU)
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> secuential_duration = end - start;
    cout << "Tiempo de ejecución secuencial (CPU): " << secuential_duration.count() << " segundos.\n";

    cout << "Resultado de la multiplicación secuencial (CPU):\n";
    print_matrix(C, N);
    cout << "\n";

    // Resetear la matriz C para el cálculo paralelo
    fill(C.begin(), C.end(), 0.0);

    // ========================================
    // Multiplicación paralela en GPU
    // ========================================
    start = chrono::high_resolution_clock::now();

    // Ejecutar en paralelo en la GPU
    matrix_multiply_parallel(A, B, C, N, gpu_queue);

    end = chrono::high_resolution_clock::now();
    chrono::duration<double> gpu_duration = end - start;
    cout << "Tiempo de ejecución paralelo (GPU): " << gpu_duration.count() << " segundos.\n";

    //cout << "Resultado de la multiplicación paralela (GPU):\n";
    //print_matrix(C, N);
    //cout << "\n";

    // Resetear la matriz C para el cálculo paralelo en CPU
    fill(C.begin(), C.end(), 0.0);

    // ========================================
    // Multiplicación paralela en CPU
    // ========================================
    start = chrono::high_resolution_clock::now();

    // Ejecutar en paralelo en la CPU
    matrix_multiply_parallel(A, B, C, N, cpu_queue);

    end = chrono::high_resolution_clock::now();
    chrono::duration<double> cpu_duration = end - start;
    cout << "Tiempo de ejecución paralelo (CPU): " << cpu_duration.count() << " segundos.\n";

   // cout << "Resultado de la multiplicación paralela (CPU):\n";
   // print_matrix(C, N);
   // cout << "\n";

   // cout << "Matriz A):\n";
    //print_matrix(A, N);
    //cout << "\n";

    //cout << "Matriz):\n";
    //print_matrix(B, N);
    //cout << "\n";
    
    return 0;
}
