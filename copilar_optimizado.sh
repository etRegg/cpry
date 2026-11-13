gcc -O3 -march=native -mtune=native \
    -fopt-info-vec-optimized \
    -fopt-info-vec-missed \
    -o entrega entrega.c -lm
