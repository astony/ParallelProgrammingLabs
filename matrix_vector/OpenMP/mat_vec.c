#include <math.h>
#include <stdio.h>
#include <stdlib.h>

void printf_vec(double *vec, int size_row) {
  int i;
  printf("\nSize vector: %d\n\n", size_row);
  for (i = 0; i < size_row; ++i) {
    printf("%lf ", vec[i]);
  }
  printf("\n\n");
}

void printf_mat(double *mat, int size_row) {
  int i;
  int size = size_row * size_row;
  printf("\nSize matrix: %d x %d\n\n", size_row, size_row);
  for (i = 0; i < size; ++i) {
    printf("%lf ", mat[i]);
    if ((i + 1) % size_row == 0)
      printf("\n");
  }
  printf("\n\n");
}

void mat_vec_row(double *a, double *x, double *y, int n) {

  int i, j;
  int index_matrix, index_row, index_matrix_result;

  // printf_mat(a, n);
  // printf_vec(x, n);

  for (i = 0; i < n; i++) {
    y[i] = 0.0;
    for (j = 0; j < n; j++) {
      index_matrix_result = i;
      index_matrix = n * i + j;
      index_row = j;
      // printf("index matrix result %d = index matrix %d * index vector %d\n",
      //       index_matrix_result, index_matrix, index_row);
      y[index_matrix_result] += a[index_matrix] * x[index_row];
    }
  }

  // printf_vec(y, n);
}

void mat_vec_col(double *a, double *x, double *y, int n) {

  int i, j;
  int index_matrix, index_row, index_matrix_result;

  // printf_mat(a, n);
  // printf_vec(x, n);

  for (i = 0; i < n; i++)
    y[i] = 0.0;

  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      index_matrix_result = j;
      index_matrix = n * j + i;
      index_row = i;
      // printf("index matrix result %d = index matrix %d * index vector %d\n",
      //       index_matrix_result, index_matrix, index_row);
      y[index_matrix_result] += a[index_matrix] * x[index_row];
    }
  }

  // printf_vec(y, n);
}
