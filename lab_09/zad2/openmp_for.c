#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

int ITERATION_NUMBER = 17;

void print_info(int i) {
  printf("w obszarze równoległym: iteracja %d, moj ID %d\n", i,
         omp_get_thread_num());
}

int main() {
  int i;

  printf("\nSchedule, size - 3\n");
#pragma omp parallel for ordered num_threads(4) schedule(static, 3) private(i)
  for (i = 0; i < ITERATION_NUMBER; ++i) {
#pragma omp ordered
    { print_info(i); }
  }

  printf("\nSchedule, size - default\n");
#pragma omp parallel for ordered num_threads(4) schedule(static) private(i)
  for (i = 0; i < ITERATION_NUMBER; ++i) {
#pragma omp ordered
    { print_info(i); }
  }

  printf("\nDynamic, size - 3\n");
#pragma omp parallel for ordered num_threads(4) schedule(dynamic, 3) private(i)
  for (i = 0; i < ITERATION_NUMBER; ++i) {
#pragma omp ordered
    { print_info(i); }
  }

  printf("\nDynamic, size - default\n");
#pragma omp parallel for ordered num_threads(4) schedule(dynamic) private(i)
  for (i = 0; i < ITERATION_NUMBER; ++i) {
#pragma omp ordered
    { print_info(i); }
  }

  printf("\nGuided, size - 3\n");
#pragma omp parallel for ordered num_threads(4) schedule(guided, 3) private(i)
  for (i = 0; i < ITERATION_NUMBER; ++i) {
#pragma omp ordered
    { print_info(i); }
  }

  printf("\nGuided, size - default\n");
#pragma omp parallel for ordered num_threads(4) schedule(guided) private(i)
  for (i = 0; i < ITERATION_NUMBER; ++i) {
#pragma omp ordered
    { print_info(i); }
  }

  return 0;
}