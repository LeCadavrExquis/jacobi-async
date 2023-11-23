#include <stdbool.h>
#include "alg.h"


void computeIteration(
        const double *x,
        double *nextX,
        double** matrix,
        const double* vector,
        int matrixSize
) {
    for (int i = 0; i < matrixSize; ++i) {
        double sumFrom1ToI = 0;
        double sumFromIToN = 0;
        for (int j = 0; j < matrixSize; j++) {
            if (j < i - 1) {
                sumFrom1ToI += matrix[i][j] * x[j];
            }
            if (i + 1 < matrixSize) {
                int shiftedJ = j + i + 1;
                sumFromIToN += matrix[i][shiftedJ] * x[shiftedJ];
            }
        }
        nextX[i] = (1 / matrix[i][i]) * (vector[i] - sumFrom1ToI - sumFromIToN);
    }
}