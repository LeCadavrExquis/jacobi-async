#include <stdbool.h>
#include <stdlib.h>
#include <math.h>

/**
 * Sprawdza czy spełniony jest warunek bezwzględnej dominacji wierszowej głównejprzekątnej
 *
 * @param m macierz
 * @param n wymiar macierzy
 */
bool isMatrixDDM(double** matrix, int n) {
    for (int i = 0; i < n; i++) {
        double sum = 0;
        for (int j = 0; j < n; j++)
            sum += fabs(matrix[i][j]);

        sum -= fabs(matrix[i][i]);

        if (fabs(matrix[i][i]) < sum)
            return false;
    }

    return true;
}

/**
 * Generuje macierz z losowymi wartościami między 0.01 i 1.01,
 * w której spełniony jest warunek bezwzględnej dominacji wierszowej głównejprzekątnej
 * @param n wymiar macierzy
 * @return wygenerowana macierz
 */
void generateDDMMatrix(double** matrix, int n) {
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++) {
            if (i != j) {
                matrix[i][j] =  0.01f + ((double)rand() / RAND_MAX);
                sum += fabs(matrix[i][j]);
            }
        }
        // Zapewnia warunek dominacji
        matrix[i][i] = sum + 1.0;
    }
}

/**
 * Generuje wektor z losowymi wartościami między -1 i 1
 * @param vector wektor o długości n
 */
void generateVector(double* vector, int n) {
    for (int i = 0; i < n; i++) {
        double toDouble = 1.0;
        vector[i] = ((toDouble * rand() / RAND_MAX) * 2) - 1;
    }
}