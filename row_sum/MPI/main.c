#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {

  int size, rank;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  int i, j;

  double *matrix;
  double *row;
  double *tmp;
  double *result;

  row = (double *)malloc(size * sizeof(double));
  tmp = (double *)malloc(size * sizeof(double));

  if (rank == 0) {
    matrix = (double *)malloc(size * size * sizeof(double));
    result = (double *)malloc(size * sizeof(double));
    for (i = 0; i < size * size; ++i) {
      matrix[i] = i + 1;
      printf("matrix[%d] = %lf\n", i, matrix[i]);
    }
  }

  for (i = 0; i < size; ++i)
    MPI_Scatter(&matrix[i * size], 1, MPI_DOUBLE, &row[i], 1, MPI_DOUBLE, 0,
                MPI_COMM_WORLD);

  for (i = 0; i < size; ++i) {
    printf("RANK: %d, row[%d] = %lf\n", rank, i, row[i]);
  }

  MPI_Reduce(row, result, size, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (rank == 0) {
    for (i = 0; i < size; ++i) {
      printf("RANK: %d, result[%d] = %lf\n", rank, i, result[i]);
    }
  }

  MPI_Finalize();
}