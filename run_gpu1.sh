#!/bin/bash
source /opt/intel/oneapi/setvars.sh > /dev/null 2>&1
/bin/echo "##" $(whoami) Se está compilando la multiplicacion de matrices en paralelo, en simultaneo Gpu y Cpu - gpu_practice1.cpp
icpx -fsycl lab/gpu_practice1.cpp
if [ $? -eq 0 ]; then ./a.out; fi