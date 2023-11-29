#include <math.h>
#include <malloc.h>
#include "alg.h"
#include "omp.h"

/**
 * Implementacja asynchronicznej wersji algorytmu Jacobiego iteracyjnego
 *
 * @param x x_i
 * @param nextX x_i+1
 */
void computeIteration(
        double* x,
        double *nextX,
        double** matrix,
        double* vector,
        int matrixSize
) {
#pragma omp parallel for
    for (int i = 0; i < matrixSize; ++i) {
        double sum = 0;
        for (int j = 0; j < matrixSize; j++) {
            if (j != i) {
                sum += matrix[i][j] * x[j];
            }
        }
        nextX[i] = (1 / matrix[i][i]) * (vector[i] - sum);
    }
}



/**
 *  Realizuje mnożenie macierzowe A[N][N] x v[N]
 *
 *  @param matrixSize -> N
 */
void computeMatrixMultiplication(double* result, double* v, double** A, int matrixSize) {
#pragma omp parallel for
    for(int i = 0; i < matrixSize; i++){
        double temp = 0;
        for(int j = 0; j< matrixSize; j++){
            temp += A[i][j] * v[j];
        }
        result[i] = temp;
    }
}


/**
 * Funkcja obliczająca normę błedu ||Ax - b||
 */
double computeNormError(double* x, double* b, double** A, int matrixSize) {
    double errorSum = 0.0;

    double* multiplicationBuffer = malloc(matrixSize * sizeof(double));
    computeMatrixMultiplication(multiplicationBuffer, x, A, matrixSize);
    for (int i = 0; i < matrixSize; i++) {
        errorSum += pow(multiplicationBuffer[i] - b[i],2);
    }
    free(multiplicationBuffer);

    return sqrt(errorSum);
}
