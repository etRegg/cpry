#!/bin/bash

# Definición de los arreglos
DIM=("512" "1024" "2048" "4096")
bs=("16"  "32" "64" "128" "256" "512")

# Bucle anidado
for ARG1 in "${DIM[@]}"; do
    for ARG2 in "${bs[@]}"; do
        # Ejecutamos sbatch definiendo el output dinámicamente
        sbatch --output="outputv1_${ARG1}_${ARG2}_%j.txt" blade.sh "$ARG1" "$ARG2"
    done
done






