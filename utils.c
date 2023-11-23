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
 * Generuje macierz z losowymi wartościami między -1 i 1,
 * w której spełniony jest warunek bezwzględnej dominacji wierszowej głównejprzekątnej
 * @param n wymiar macierzy
 * @return wygenerowana macierz
 */
void generateDDMMatrix(double** matrix, int n) {
    for (int i = 0; i < n; i++) {
        double sum = 0.0;
        for (int j = 0; j < n; j++) {
            double toDouble = 1.0;
            if (i != j) {
                matrix[i][j] = ((toDouble * rand() / RAND_MAX) * 2) - 1;
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

/**
 * Funkcja obliczająca różnicę norm x dla kolejnych iteracji w celu zdefiniowania warunku stopu
 * @return różnica norm wektorów
 */
double vectorNormDifference(double* x1, double* x2, int vectorSize) {
    double normDifference = 0.0;
    for (int i = 0; i < vectorSize; i++) {
        normDifference += pow(x1[i] - x2[i], 2);
    }
    normDifference = sqrt(normDifference);

    return normDifference;
}
