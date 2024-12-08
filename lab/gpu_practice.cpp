 Arquitectura2labgpu_practice.cpp
#include CLsycl.hpp
#include iostream
#include vector
#include chrono   Para medir tiempos

using namespace std;
using namespace sycl;

 Función secuencial de multiplicación de matrices
void matrix_multiply_sequential(const stdvectorfloat& A, const stdvectorfloat& B, 
                                 stdvectorfloat& C, size_t N) {
    for (size_t i = 0; i  N; i++) {
        for (size_t j = 0; j  N; j++) {
            float sum = 0;
            for (size_t k = 0; k  N; k++) {
                sum += A[i  N + k]  B[k  N + j];
            }
            C[i  N + j] = sum;
        }
    }
}

 Función paralela de multiplicación de matrices con SYCL
void matrix_multiply_parallel(const stdvectorfloat& A, const stdvectorfloat& B, 
                              stdvectorfloat& C, size_t N, queue& q) {
    buffer a_buf(A.data(), range1(N  N));
    buffer b_buf(B.data(), range1(N  N));
    buffer c_buf(C.data(), range1(N  N));

    q.submit([&](handler& h) {
        auto a = a_buf.get_accessaccessmoderead(h);
        auto b = b_buf.get_accessaccessmoderead(h);
        auto c = c_buf.get_accessaccessmodewrite(h);

        h.parallel_for(range2(N, N), [=](id2 idx) {
            size_t row = idx[0];
            size_t col = idx[1];
            float sum = 0;

            for (size_t k = 0; k  N; k++) {
                sum += a[row  N + k]  b[k  N + col];
            }
            c[row  N + col] = sum;
        });
    });

    q.wait();  Sincroniza las operaciones
}

 Función para imprimir matrices
void print_matrix(const stdvectorfloat& matrix, size_t N) {
    for (size_t i = 0; i  N; i++) {
        for (size_t j = 0; j  N; j++) {
            stdcout  matrix[i  N + j]   ;
        }
        stdcout  n;
    }
}

int main() {
    const size_t N = 4;  Tamaño de la matriz (NxN)
    stdvectorfloat A(N  N, 1.0);  Inicializa con 1s
    stdvectorfloat B(N  N, 2.0);  Inicializa con 2s
    stdvectorfloat C(N  N, 0.0);  Resultado

     Crear una cola SYCL con el selector predeterminado
    queue q{sycldefault_selector_v};
    stdcout  Usando dispositivo   q.get_device().get_infoinfodevicename()  n;

     Medir el tiempo para la multiplicación secuencial
    auto start = chronohigh_resolution_clocknow();
    matrix_multiply_sequential(A, B, C, N);
    auto end = chronohigh_resolution_clocknow();
    chronodurationdouble secuential_duration = end - start;
    cout  Tiempo de ejecución secuencial   secuential_duration.count()   segundos.n;

     Imprimir el resultado de la multiplicación secuencial
    cout  Resultado de la multiplicación secuencialn;
    print_matrix(C, N);

     Resetear la matriz C para el cálculo paralelo
    fill(C.begin(), C.end(), 0.0);

     Medir el tiempo para la multiplicación paralela (con SYCL)
    start = chronohigh_resolution_clocknow();
    matrix_multiply_parallel(A, B, C, N, q);
    end = chronohigh_resolution_clocknow();
    chronodurationdouble parallel_duration = end - start;
    cout  Tiempo de ejecución paralelo (SYCL)   parallel_duration.count()   segundos.n;

     Imprimir el resultado de la multiplicación paralela
    cout  Resultado de la multiplicación paralela (SYCL)n;
    print_matrix(C, N);

    return 0;
}
