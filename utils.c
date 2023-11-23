#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

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
