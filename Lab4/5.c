// Name: Guanghan Li
// Date: 2025-10-13
// Title: Lab4 – Step 5
// Description: This program uses threads to multiply two matrices. One thread is created for each element of the result matrix.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Global matrix pointers and sizes
double **leftMat, **rightMat, **resultMat;
int N, M, L;

// Structure for passing (row, col) to a thread
typedef struct {
    int row;
    int col;
} CellIndex;

double **alloc_matrix(int rows, int cols) {
    double **mat = (double **)malloc(rows * sizeof(double *));
    for (int r = 0; r < rows; r++) {
        mat[r] = (double *)malloc(cols * sizeof(double));
        for (int c = 0; c < cols; c++)
            mat[r][c] = rand() % 10; // values 0-9
    }
    return mat;
}

void print_matrix(int rows, int cols, double **mat) {
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < cols; c++)
            printf("%5.0f ", mat[r][c]);
        printf("\n");
    }
}

// Thread function to compute one element
void *compute_cell(void *arg) {
    CellIndex *idx = (CellIndex *)arg;
    int r = idx->row, c = idx->col;

    double acc = 0.0;
    for (int k = 0; k < M; k++)
        acc += leftMat[r][k] * rightMat[k][c];

    resultMat[r][c] = acc;
    free(idx);
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s N M L\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]);
    M = atoi(argv[2]);
    L = atoi(argv[3]);

    srand((unsigned)time(NULL));
    leftMat   = alloc_matrix(N, M);
    rightMat  = alloc_matrix(M, L);
    resultMat = alloc_matrix(N, L);

    // One thread per element in resultMat
    pthread_t threads[N * L];
    int t = 0;
    for (int r = 0; r < N; r++) {
        for (int c = 0; c < L; c++) {
            CellIndex *idx = (CellIndex *)malloc(sizeof(CellIndex));
            idx->row = r;
            idx->col = c;
            pthread_create(&threads[t++], NULL, compute_cell, idx);
        }
    }

    for (int i = 0; i < N * L; i++)
        pthread_join(threads[i], NULL);

    printf("Matrix A:\n");
    print_matrix(N, M, leftMat);
    printf("\nMatrix B:\n");
    print_matrix(M, L, rightMat);
    printf("\nMatrix C (Result):\n");
    print_matrix(N, L, resultMat);

    // Free memory
    for (int r = 0; r < N; r++) free(leftMat[r]);
    for (int r = 0; r < M; r++) free(rightMat[r]);
    for (int r = 0; r < N; r++) free(resultMat[r]);
    free(leftMat); free(rightMat); free(resultMat);

    return 0;
}
