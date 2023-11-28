#ifndef JACOBI_ASYNC_ALG_H
#define JACOBI_ASYNC_ALG_H

void computeIteration(double *x, double *nextX, double** matrix, double* vector, int matrixSize);
double computeNormError(double* x, double* b, double** A, int matrixSize);

#endif
