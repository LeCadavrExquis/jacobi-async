#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <mpi/mpi.h>
#include "utils.h"

void parallelComputeIteration(double* localX, double* globalX, double** matrix, double* vector, int matrixSize, int localSize, int world_rank) {
    // Wysyłanie i odbieranie aktualnych wartości x od wszystkich procesów
    MPI_Allgather(localX, localSize, MPI_DOUBLE, globalX, localSize, MPI_DOUBLE, MPI_COMM_WORLD);

    for (int i = 0; i < localSize; ++i) {
        double sum = 0;
        int globalIndex = world_rank * localSize + i;
        for (int j = 0; j < matrixSize; j++) {
            if (j != globalIndex) {
                sum += matrix[i][j] * globalX[j];
            }
        }
        localX[i] = (1 / matrix[i][globalIndex]) * (vector[globalIndex] - sum);
    }
}

double computeLocalNormError(double* localX, double* globalX, double* b, double** A, int localSize, int world_rank) {
    double localErrorSum = 0.0;
    int globalIndexStart = world_rank * localSize;

    for (int i = 0; i < localSize; i++) {
        int globalIndex = globalIndexStart + i;
        double axMinusB = 0;
        for (int j = 0; j < localSize; j++) {
            axMinusB += A[i][j] * globalX[j];
        }
        localErrorSum += pow(axMinusB - b[globalIndex], 2);
    }

    return localErrorSum;
}

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);

    int world_rank, world_size;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    int MATRIX_SIZE = 1000;
    int localSize = MATRIX_SIZE / world_size; // Zakładamy, że MATRIX_SIZE jest podzielne przez world_size

    double **matrix = malloc(localSize * sizeof(double *));
    for (int i = 0; i < localSize; i++) {
        matrix[i] = malloc(MATRIX_SIZE * sizeof(double));
    }
    generateDDMMatrix(matrix, MATRIX_SIZE); // Funkcja do zmodyfikowania, aby generować tylko lokalną część macierzy

    double *vector = malloc(MATRIX_SIZE * sizeof(double));
    generateVector(vector, MATRIX_SIZE); // Funkcja do zmodyfikowania, aby generować tylko lokalną część wektora

    double *localX = malloc(localSize * sizeof(double));
    double *globalX = malloc(MATRIX_SIZE * sizeof(double));

    for (int i = 0; i < localSize; i++) {
        localX[i] = 1;
    }

    struct timespec startTimestamp;
    clock_gettime(CLOCK_REALTIME, &startTimestamp);

    double STOP_ERROR = 0.1;
    double relativeError = 1;
    int iterationCount = 0;
    while (relativeError > STOP_ERROR) {
        parallelComputeIteration(localX, globalX, matrix, vector, MATRIX_SIZE, localSize, world_rank);

        double localError = computeLocalNormError(localX, globalX, vector, matrix, localSize, world_rank);
        double totalError;
        MPI_Allreduce(&localError, &totalError, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        relativeError = sqrt(totalError);

        memcpy(localX, globalX + world_rank * localSize, localSize * sizeof(double));
        iterationCount++;

        if (world_rank == 0) { // Tylko proces główny drukuje status
            printf("i = %d, err = %f\n", iterationCount, relativeError);
        }
    }

    struct timespec stopTimestamp;
    clock_gettime(CLOCK_REALTIME, &stopTimestamp);

    long long timeElapsedNs = (long long)(stopTimestamp.tv_sec - startTimestamp.tv_sec) * 1e9 + (stopTimestamp.tv_nsec - startTimestamp.tv_nsec);
    long timeElapsedS = timeElapsedNs / 1e9;
    long timeElapsedMs = (timeElapsedNs % (long long)1e9) / 1e6;

    if (world_rank == 0) {
        printf("Obliczenia zakończone po %d iteracjach\n", iterationCount);
        printf("Czas obliczeń: %ld s i %ld ms\n", timeElapsedS, timeElapsedMs);
    }

    for (int i = 0; i < localSize; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);
    free(localX);
    free(globalX);

    MPI_Finalize();
    return 0;
}
