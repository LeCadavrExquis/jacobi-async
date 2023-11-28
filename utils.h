#ifndef JACOBI_ASYNC_UTILS_H
#define JACOBI_ASYNC_UTILS_H

#include <stdbool.h>
#include <math.h>

bool isMatrixDDM(double** matrix, int n);
void generateDDMMatrix(double** matrix, int n);
void generateVector(double* vector, int n);

#endif