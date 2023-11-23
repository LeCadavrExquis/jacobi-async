#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

int main() {
    int matrixSize = 10;
    double **matrix = malloc(matrixSize * sizeof(double *));
    for (int i = 0; i < 10; i++) {
        matrix[i] = malloc(10 * sizeof(double));
    }
    generateDDMMatrix(matrix, 10);
    printf("Generowanie macierzy %d x %d\n", matrixSize, matrixSize);

    for (int i = 0; i < 10; i++) {
        free(matrix[i]);
    }
    free(matrix);
    return 0;
}
