#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 512

void fill_matrix(double mat[N][N]) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            mat[i][j] = rand() % 10;
}

void zero_matrix(double mat[N][N]) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            mat[i][j] = 0;
}

void serial_matmul(double A[N][N], double B[N][N], double C[N][N]) {
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            for (int k = 0; k < N; ++k)
                C[i][j] += A[i][k] * B[k][j];
}

int main() {
    double A[N][N], B[N][N], C[N][N];
    srand(time(NULL));

    fill_matrix(A);
    fill_matrix(B);
    zero_matrix(C);

    clock_t start = clock();
    serial_matmul(A, B, C);
    clock_t end = clock();

    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf(" Serial Execution Time: %.6f sec\n", time_taken);
    return 0;
}

