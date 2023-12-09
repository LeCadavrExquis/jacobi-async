#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/time.h>
#include "utils.h"
#include "mpi.h"
#include "alg.h"

#define N (1024 * 1024 * 1)

int main(int argc, char* argv[]) {
    int process_Rank, size_Of_Cluster;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    printf("Hello World from process %d of %d\n", process_Rank, size_Of_Cluster);
    int MATRIX_SIZE = 1000;

    int size, rank;
    struct timeval start, end;
    char hostname[256];
    int hostname_len;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Get_processor_name(hostname, &hostname_len);

    // Allocate a 1 MiB buffer
    char* buffer = malloc(MATRIX_SIZE * sizeof(char));

    // Communicate along the ring
    if (rank == 0) {
        gettimeofday(&start, NULL);
        printf("Rank %d (running on '%s'): sending the message rank %d\n", rank, hostname, 1);
        MPI_Send(buffer, N, MPI_BYTE, 1, 1, MPI_COMM_WORLD);
        MPI_Recv(buffer, N, MPI_BYTE, size - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d (running on '%s'): received the message from rank %d\n", rank, hostname, size - 1);
        gettimeofday(&end, NULL);
        printf("%f\n", (end.tv_sec * 1000000.0 + end.tv_usec - start.tv_sec * 1000000.0 - start.tv_usec) / 1000000.0);

    } else {
        MPI_Recv(buffer, N, MPI_BYTE, rank - 1, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Rank %d (running on '%s'): receive the message and sending it to rank %d\n", rank, hostname,
               (rank + 1) % size);
        MPI_Send(buffer, N, MPI_BYTE, (rank + 1) % size, 1, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;

    printf("Generowanie macierzy %d x %d\n", MATRIX_SIZE, MATRIX_SIZE);
    double **matrix = malloc(MATRIX_SIZE * sizeof(double *));
    for (int i = 0; i < MATRIX_SIZE; i++) {
        matrix[i] = malloc(MATRIX_SIZE * sizeof(double));
    }
    generateDDMMatrix(matrix, MATRIX_SIZE);

    int VECTOR_SPLIT = 2;
    double *vector = malloc(MATRIX_SIZE * sizeof(double));
    generateVector(vector, MATRIX_SIZE);

    double *currentX = malloc(MATRIX_SIZE * sizeof(double));
    for (int i = 0; i < MATRIX_SIZE; i++) {
        currentX[i] = 1;
    }
    struct timespec startTimestamp;
    clock_gettime(CLOCK_REALTIME, &startTimestamp);
    double *nextX = malloc(MATRIX_SIZE * sizeof(double));

    double STOP_ERROR = 0.1;
    double relativeError = 1;
    int iterationCount = 0;
    while (relativeError > STOP_ERROR) {
        computeIteration(currentX, nextX, matrix, vector, MATRIX_SIZE);

        relativeError = computeNormError(nextX, vector, matrix, MATRIX_SIZE);

        memcpy(currentX, nextX, MATRIX_SIZE * sizeof(double));
        iterationCount++;

        printf("i = %d, err = %f\n", iterationCount, relativeError);
    }
    struct timespec stopTimestamp;
    clock_gettime(CLOCK_REALTIME, &stopTimestamp);

    long timeElapsedNs = (stopTimestamp.tv_sec - startTimestamp.tv_sec) * 1e9 + (stopTimestamp.tv_nsec - startTimestamp.tv_nsec);
    long timeElapsedS = timeElapsedNs / 1e9;
    long timeElapsedMs = (timeElapsedNs % (long)1e9) / 1e6;

    printf("Obliczenia zakończone po %d iteracjach\n", iterationCount);
    printf("Czas obliczeń: %ld s i %ld ms\n", timeElapsedS, timeElapsedMs);

    for (int i = 0; i < 10; i++) {
        free(matrix[i]);
    }
    free(matrix);
    free(vector);

    return 0;
}
