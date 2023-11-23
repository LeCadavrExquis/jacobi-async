#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"
#include "alg.h"

int main() {
    int MATRIX_SIZE = 10000;
    double **matrix = malloc(MATRIX_SIZE * sizeof(double *));
    for (int i = 0; i < MATRIX_SIZE; i++) {
        matrix[i] = malloc(MATRIX_SIZE * sizeof(double));
    }
    generateDDMMatrix(matrix, MATRIX_SIZE);
    printf("Generowanie macierzy %d x %d\n", MATRIX_SIZE, MATRIX_SIZE);

    int VECTOR_SPLIT = 2;
    double *vector = malloc(MATRIX_SIZE * sizeof(double *));
    generateVector(vector, MATRIX_SIZE);

    double *currentX = malloc(MATRIX_SIZE * sizeof(double *));
    for (int i = 0; i < 10; i++) {
        currentX[i] = 1;
    }
    struct timespec startTimestamp;
    clock_gettime(CLOCK_REALTIME, &startTimestamp);
    double *nextX = malloc(MATRIX_SIZE * sizeof(double *));
    double STOP_ERROR = 0.0000001;
    double relativeError = 1;
    int iterationCount = 0;
    while (relativeError > STOP_ERROR) {
        computeIteration(currentX, nextX, matrix, vector, MATRIX_SIZE);

        relativeError = vectorNormDifference(currentX, nextX, MATRIX_SIZE);

        for (int i = 0; i < MATRIX_SIZE; i++) {
            currentX[i] = nextX[i];
        }
        iterationCount++;
    }
    struct timespec stopTimestamp;
    clock_gettime(CLOCK_REALTIME, &stopTimestamp);

    long timeElapsedNs = (stopTimestamp.tv_sec - startTimestamp.tv_sec) * 1e9 + (stopTimestamp.tv_nsec - startTimestamp.tv_nsec);
    long timeElapsedS = timeElapsedNs / 1e9;
    long timeElapsedMs = (timeElapsedNs % (long)1e9) / 1e6;

    printf("wektor wynikowy: [");
    for (int i = 0; i < MATRIX_SIZE; ++i) {
        printf("%f", currentX[i]);
    }
    printf("]\n");
    printf("Obliczenia zakończone po %d iteracjach\n", iterationCount);
    printf("Czas obliczeń: %ld s i %ld ms\n", timeElapsedS, timeElapsedMs);
    printf("Błąd norm: %f\n", relativeError);


    for (int i = 0; i < 10; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);

    return 0;
}
