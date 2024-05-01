//Student submissions will appear here
// Standard libraries
#include <stdio.h>
#include <math.h>



//User libraries:
#include "BOARD.h"

void MatrixPrint(float mat[3][3]);
int MatrixEquals(float mat1[3][3], float mat2[3][3]);
void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]);
void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]);
void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]);
void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]);
float MatrixTrace(float mat[3][3]);
void MatrixTranspose(float mat[3][3], float result[3][3]);
void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]);
float MatrixDeterminant(float mat[3][3]);
void MatrixInverse(float mat[3][3], float result[3][3]);

void MatrixPrint(float mat[3][3]) {
    printf(" -----------------------\n");
    for (int i = 0; i < 3; i++) {
        printf("|");
        for (int j = 0; j < 3; j++) {
            printf("%6.2f |", mat[i][j]);
        }
        printf("\n");
    }
    printf(" -----------------------\n");
}

int MatrixEquals(float mat1[3][3], float mat2[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (mat1[i][j] != mat2[i][j]) {
                return 0;
            }
        }
    }
    return 1;
}

void MatrixAdd(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = mat1[i][j] + mat2[i][j];
        }
    }
}

void MatrixMultiply(float mat1[3][3], float mat2[3][3], float result[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = 0;
            for (int k = 0; k < 3; k++) {
                result[i][j] += mat1[i][k] * mat2[k][j];
            }
        }
    }
}

void MatrixScalarAdd(float x, float mat[3][3], float result[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = mat[i][j] + x;
        }
    }
}

void MatrixScalarMultiply(float x, float mat[3][3], float result[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = mat[i][j] * x;
        }
    }
}

float MatrixTrace(float mat[3][3]) {
    float trace = 0;
    for (int i = 0; i < 3; i++) {
        trace += mat[i][i];
    }
    return trace;
}

void MatrixTranspose(float mat[3][3], float result[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            result[i][j] = mat[j][i];
        }
    }
}

void MatrixSubmatrix(int i, int j, float mat[3][3], float result[2][2]) {
    int rowIndex = 0;
    for (int row = 0; row < 3; row++) {
        if (row == i) continue;
        int colIndex = 0;
        for (int col = 0; col < 3; col++) {
            if (col == j) continue;
            result[rowIndex][colIndex] = mat[row][col];
            colIndex++;
        }
        rowIndex++;
    }
}

float MatrixDeterminant(float mat[3][3]) {
    float det = mat[0][0] * (mat[1][1] * mat[2][2] - mat[1][2] * mat[2][1])
            - mat[0][1] * (mat[1][0] * mat[2][2] - mat[1][2] * mat[2][0])
            + mat[0][2] * (mat[1][0] * mat[2][1] - mat[1][1] * mat[2][0]);
    return det;
}

void MatrixInverse(float mat[3][3], float result[3][3]) {
    float det = MatrixDeterminant(mat);
    if (det == 0) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                result[i][j] = 0;
            }
        }
        return;
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            float submatrix[2][2];
            MatrixSubmatrix(i, j, mat, submatrix);
            float cofactor = ((i + j) % 2 == 0 ? 1 : -1) * (submatrix[0][0] * submatrix[1][1] - submatrix[0][1] * submatrix[1][0]);
            result[j][i] = cofactor / det;
        }
    }
}
