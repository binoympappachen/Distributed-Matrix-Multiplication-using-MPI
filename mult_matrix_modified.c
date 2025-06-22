#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

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

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int rows_per_proc = N / size;
    double A[N][N], B[N][N], C[N][N];
    double A_local[rows_per_proc][N], C_local[rows_per_proc][N];

    if (rank == 0) {
        srand(42);
        fill_matrix(A);
        fill_matrix(B);
    }

    MPI_Bcast(B, N * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Scatter(A, rows_per_proc * N, MPI_DOUBLE, A_local, rows_per_proc * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double start = MPI_Wtime();

    for (int i = 0; i < rows_per_proc; ++i)
        for (int j = 0; j < N; ++j) {
            C_local[i][j] = 0;
            for (int k = 0; k < N; ++k)
                C_local[i][j] += A_local[i][k] * B[k][j];
        }

    double end = MPI_Wtime();
    double local_time = end - start;

    MPI_Gather(C_local, rows_per_proc * N, MPI_DOUBLE, C, rows_per_proc * N, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("🔁 MPI Parallel Execution Time: %.6f sec using %d processes\n", local_time, size);
    }

    MPI_Finalize();
    return 0;
}

