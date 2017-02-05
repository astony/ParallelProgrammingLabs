#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define WYMIAR 4

int main() {
  double *matrix;
  double *result;

  matrix = (double *)malloc(WYMIAR * WYMIAR * sizeof(double));
  result = (double *)malloc(WYMIAR * sizeof(double));

  int i, j, matrix_index, result_index;

  for (i = 0; i < WYMIAR * WYMIAR; ++i) {
    matrix[i] = (i + 1) % WYMIAR;
    printf("matrix[%d] = %lf\n", i, matrix[i]);
  }

  omp_set_num_threads(WYMIAR);

  // Wierszowo
  for (i = 0; i < WYMIAR; i++) {
    result[i] = 0.0f;
  }

#pragma omp parallel for ordered default(none)                                 \
    shared(result, matrix) private(i, j, matrix_index, result_index)
  for (i = 0; i < WYMIAR; i++) {
    for (j = 0; j < WYMIAR; j++) {
      result_index = i;
      matrix_index = WYMIAR * i + j;
#pragma omp ordered
      printf("result index %d += matrix index %d, wątek %d\n", result_index,
             matrix_index, omp_get_thread_num());
#pragma omp critical
      result[result_index] += matrix[matrix_index];
    }
  }

  for (i = 0; i < WYMIAR; ++i) {
    printf("result[%d] = %lf\n", i, result[i]);
  }

  // Kolumnowo
  for (i = 0; i < WYMIAR; i++) {
    result[i] = 0.0f;
  }

#pragma omp parallel for ordered default(none)                                 \
    shared(result, matrix) private(i, j, matrix_index, result_index)
  for (i = 0; i < WYMIAR; i++) {
    for (j = 0; j < WYMIAR; j++) {
      result_index = j;
      matrix_index = WYMIAR * j + i;
#pragma omp ordered
      printf("result index %d += matrix index %d, wątek %d\n", result_index,
             matrix_index, omp_get_thread_num());
#pragma omp critical
      result[result_index] += matrix[matrix_index];
    }
  }

  for (i = 0; i < WYMIAR; ++i) {
    printf("result[%d] = %lf\n", i, result[i]);
  }

  return 0;
}
