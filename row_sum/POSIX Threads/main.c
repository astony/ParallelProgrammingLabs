#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define WYMIAR 4

pthread_t thread[WYMIAR];
int thread_id[WYMIAR];

pthread_mutex_t mutex;

double *matrix;
double *result;

void row_row_sum(void *arg) {
  int id = *((int *)arg);

  int i, matrix_index, result_index;
  result_index = id;
  for (i = 0; i < WYMIAR; ++i) {
    matrix_index = WYMIAR * id + i;
    pthread_mutex_lock(&mutex);
    result[result_index] += matrix[matrix_index];
    pthread_mutex_unlock(&mutex);
  }
}

void row_col_sum(void *arg) {
  int id = *((int *)arg);

  int i, matrix_index, result_index;
  for (i = 0; i < WYMIAR; ++i) {
    result_index = i;
    matrix_index = WYMIAR * i + id;
    pthread_mutex_lock(&mutex);
    result[result_index] += matrix[matrix_index];
    pthread_mutex_unlock(&mutex);
  }
}

int main() {
  int i;

  matrix = (double *)malloc(WYMIAR * WYMIAR * sizeof(double));
  result = (double *)malloc(WYMIAR * sizeof(double));

  for (i = 0; i < WYMIAR * WYMIAR; ++i) {
    matrix[i] = (i + 1) % WYMIAR;
    printf("matrix[%d] = %lf\n", i, matrix[i]);
  }

  // Wierszowo
  for (i = 0; i < WYMIAR; ++i) {
    result[i] = 0.0;
  }

  for (i = 0; i < WYMIAR; ++i) {
    thread_id[i] = i;
    pthread_create(&thread[i], NULL, row_row_sum, &thread_id[i]);
  }

  for (i = 0; i < WYMIAR; ++i) {
    pthread_join(thread[i], NULL);
  }

  for (i = 0; i < WYMIAR; ++i) {
    printf("result[%d] = %lf\n", i, result[i]);
  }

  // Kolumnowo
  for (i = 0; i < WYMIAR; ++i) {
    result[i] = 0.0;
  }

  for (i = 0; i < WYMIAR; ++i) {
    thread_id[i] = i;
    pthread_create(&thread[i], NULL, row_col_sum, &thread_id[i]);
  }

  for (i = 0; i < WYMIAR; ++i) {
    pthread_join(thread[i], NULL);
  }

  for (i = 0; i < WYMIAR; ++i) {
    printf("result[%d] = %lf\n", i, result[i]);
  }

  pthread_exit(0);
}