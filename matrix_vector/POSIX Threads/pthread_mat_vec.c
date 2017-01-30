#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define WYMIAR 3
#define ROZMIAR (WYMIAR * WYMIAR)

double *matrix;
double *vector;
double *result;

pthread_t thread[WYMIAR];
pthread_mutex_t mutex;
int thread_id[WYMIAR];

void printf_table(double *table, int size) {
  int i;
  printf("Size: %d\n\n", size);
  for (i = 0; i < size; ++i) {
    printf("%lf ", table[i]);
    if ((i + 1) % WYMIAR == 0)
      printf("\n");
  }
  printf("\n\n");
}

void matrix_row(void *arg) {
  int id = *((int *)arg);
  // printf("ID: %d\n", id);

  int i, index_matrix, index_vector, index_vector_result;

  index_vector_result = id;
  for (i = 0; i < WYMIAR; ++i) {
    index_matrix = WYMIAR * id + i;
    index_vector = i;
    printf("index vector result %d, index matrix %d * index vector %d\n",
           index_vector_result, index_matrix, index_vector);
    pthread_mutex_lock(&mutex);
    result[index_vector_result] += matrix[index_matrix] * vector[index_vector];
    pthread_mutex_unlock(&mutex);
  }
}

void matrix_col(void *arg) {
  int id = *((int *)arg);
  // printf("ID: %d\n", id);

  int i, index_matrix, index_vector, index_vector_result;
  double sum = 0.0;

  for (i = 0; i < WYMIAR; ++i) {
    index_vector_result = i;
    index_matrix = WYMIAR * i + id;
    index_vector = id;
    printf("index vector result %d, index matrix %d * index vector %d\n",
           index_vector_result, index_matrix, index_vector);
    pthread_mutex_lock(&mutex);
    result[index_vector_result] += matrix[index_matrix] * vector[index_vector];
    pthread_mutex_unlock(&mutex);
  }
}

int main() {
  int i;

  matrix = (double *)malloc(ROZMIAR * sizeof(double));
  for (i = 0; i < ROZMIAR; ++i)
    matrix[i] = i;
  printf_table(matrix, ROZMIAR);

  vector = (double *)malloc(WYMIAR * sizeof(double));
  for (i = 0; i < WYMIAR; ++i)
    vector[i] = (WYMIAR - i);
  printf_table(vector, WYMIAR);

  result = (double *)malloc(WYMIAR * sizeof(double));
  for (i = 0; i < WYMIAR; ++i)
    result[i] = 0.0;

  pthread_mutex_init(&mutex, NULL);

  // Wierszowo
  for (i = 0; i < WYMIAR; ++i) {
    thread_id[i] = i;
    pthread_create(&thread[i], NULL, matrix_row, &thread_id[i]);
  }

  for (i = 0; i < WYMIAR; ++i)
    pthread_join(thread[i], NULL);

  printf_table(result, WYMIAR);

  // Kolumnowo
  for (i = 0; i < WYMIAR; ++i)
    result[i] = 0.0;

  for (i = 0; i < WYMIAR; ++i) {
    thread_id[i] = i;
    pthread_create(&thread[i], NULL, matrix_col, &thread_id[i]);
  }

  for (i = 0; i < WYMIAR; ++i)
    pthread_join(thread[i], NULL);

  printf_table(result, WYMIAR);

  pthread_exit(NULL);
}