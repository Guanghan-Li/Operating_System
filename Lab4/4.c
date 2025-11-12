// Name: Guanghan Li
// Date: 2025-10-13
// Title: Lab4 – Step 4
// Description: This program uses threads to multiply two matrices. One thread is created for each row of the result matrix.
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

double **leftMat, **rightMat, **resultMat;
int N, M, L;

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

// Thread function to compute one row's values in resultMat
void *compute_row(void *arg) {
    int r = (int)(size_t)arg;
    for (int c = 0; c < L; c++) {
        double acc = 0;
        for (int k = 0; k < M; k++)
            acc += leftMat[r][k] * rightMat[k][c];
        resultMat[r][c] = acc;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s N M L\n", argv[0]);
        return 1;
    }

    N = atoi(argv[1]); // N
    M = atoi(argv[2]); // M
    L = atoi(argv[3]); // L

    srand((unsigned)time(NULL));
    leftMat = alloc_matrix(N, M);
    rightMat = alloc_matrix(M, L);
    resultMat = alloc_matrix(N, L);

    pthread_t workers[N]; // one per row
    for (int r = 0; r < N; r++)
        pthread_create(&workers[r], NULL, compute_row, (void *)(size_t)r);

    for (int r = 0; r < N; r++)
        pthread_join(workers[r], NULL);

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
