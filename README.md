Proyecto OneAPI: Multiplicación de matrices
  Este proyecto está diseñado para realizar la multiplicación de matrices utilizando SYCL, un modelo de programación paralela que permite aprovechar
  dispositivos como GPUs para mejorar el rendimiento. Además, incluye un conjunto de scripts que facilitan la ejecución en Intel DevCloud, un entorno 
  de cómputo de alto rendimiento que permite ejecutar trabajos en la infraestructura de Intel.

Contenido del Proyecto
  gpu_practice.cpp: Implementa la multiplicación de matrices en paralelo utilizando SYCL. En el código:
        •	Se crean dos matrices A y B de tamaño N x N con valores iniciales de 1.0 y 2.0, respectivamente.
        •	Se crea una cola SYCL (queue q) que selecciona automáticamente el dispositivo de cómputo disponible (probablemente una GPU si está presente).
        •	La función matrix_multiply realiza la multiplicación de matrices utilizando buffers y el parallel_for de SYCL para distribuir la operación de forma paralela.
        •	Al final, el código imprime el resultado de la multiplicación de matrices.
  
  run_gpu.sh: Un script de Bash que compila y ejecuta el código en un entorno Intel DevCloud, donde se ejecuta el código aprovechando la infraestructura de cómputo de alto rendimiento disponible.
  
  q: Un script de Bash que envía el trabajo a la cola de trabajos de Intel DevCloud y realiza un seguimiento del progreso del trabajo, esperando la generación del archivo de salida con los resultados.
  
  README.md: Un archivo que proporciona información detallada sobre el proyecto.
