#!/bin/bash
#SBATCH --job-name=hostname_test       # Nombre del trabajo
#SBATCH --partition=Blade              # Partición del clúster (ajústalo a la que quieras usar)
#SBATCH --nodes=1                      # Número de nodos
#SBATCH --ntasks=1                     # Una única tarea
#SBATCH --cpus-per-task=1              # Un solo CPU para la tarea
#SBATCH --exclusive                    # Modo Exclusivo
#SBATCH --time=00:05:00                # Tiempo límite
#SBATCH --output=output_%j.txt         # Archivo de salida
# Comando a ejecutar
./entrega $1  $2
