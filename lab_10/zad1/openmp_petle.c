#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define WYMIAR 4

int main() {
  double a[WYMIAR][WYMIAR];
  int n, i, j;

  for (i = 0; i < WYMIAR; i++)
    for (j = 0; j < WYMIAR; j++)
      a[i][j] = 1.02 * i + 1.01 * j;

  n = WYMIAR;

  double suma = 0.0;
  for (i = 0; i < WYMIAR; i++)
    for (j = 0; j < WYMIAR; j++)
      suma += a[i][j];

  printf("Suma wyrazów tablicy: %lf\n\n", suma);

  omp_set_nested(1);
  omp_set_num_threads(WYMIAR);

  double suma_parallel = 0.0;
#pragma omp parallel for ordered default(none)                                 \
    num_threads(WYMIAR) private(i, j) shared(a) reduction(+ : suma_parallel)
  for (i = 0; i < WYMIAR; i++)
    for (j = 0; j < WYMIAR; j++) {
#pragma omp ordered
      {
        printf("wiersz %d, kolumna %d, wątek %d\n", i, j, omp_get_thread_num());
        suma_parallel += a[i][j];
      }
    }

  printf("Suma wyrazów tablicy równolegle (wierszowo): %lf\n\n", suma_parallel);

  double suma_parallel_tab[WYMIAR];
  for (i = 0; i < WYMIAR; i++)
    suma_parallel_tab[i] = 0.0f;
  suma_parallel = 0.0f;
  for (i = 0; i < WYMIAR; i++) {
#pragma omp parallel for ordered default(none) num_threads(WYMIAR) private(j)  \
    shared(i, a, suma_parallel_tab)
    for (j = 0; j < WYMIAR; j++) {
#pragma omp ordered
      {
#pragma omp critical
        {
          printf("wiersz %d, kolumna %d, wątek %d\n", i, j,
                 omp_get_thread_num());
          suma_parallel_tab[i] += a[i][j];
        }
      }
    }
  }

  for (i = 0; i < WYMIAR; i++)
    suma_parallel += suma_parallel_tab[i];

  printf("Suma wyrazów tablicy równolegle (wierszowo - local): %lf\n\n",
         suma_parallel);

  suma_parallel = 0.0;

  for (i = 0; i < WYMIAR; i++) {
#pragma omp parallel for ordered default(none) num_threads(WYMIAR) private(j)  \
    shared(i, a) reduction(+ : suma_parallel)
    for (j = 0; j < WYMIAR; j++) {
#pragma omp ordered
      {
        printf("wiersz %d, kolumna %d, wątek %d\n", i, j, omp_get_thread_num());
        suma_parallel += a[i][j];
      }
    }
  }

  printf("Suma wyrazów tablicy równolegle (kolumnowo): %lf\n\n", suma_parallel);

  suma_parallel = 0.0;
  double global_parallel = 0.0;
#pragma omp parallel for ordered default(none) num_threads(WYMIAR) private(    \
    i, j, suma_parallel) shared(a, global_parallel)
  for (i = 0; i < WYMIAR; i++) {
    suma_parallel = 0.0;
    for (j = 0; j < WYMIAR; j++) {
#pragma omp ordered
      {
        printf("wiersz %d, kolumna %d, wątek %d\n", i, j, omp_get_thread_num());
#pragma omp atomic
        suma_parallel += a[i][j];
      }
#pragma omp critical
      {
        if (j == WYMIAR - 1) {
#pragma omp barier
          global_parallel += suma_parallel;
        }
      }
    }
  }

  printf("Suma wyrazów tablicy równolegle (kolumnowo - private): %lf\n\n",
         global_parallel);

  suma_parallel = 0.0;
#pragma omp parallel for ordered default(none)                                 \
    shared(a, i, j) reduction(+ : suma_parallel) schedule(static, 10)
  for (i = 0; i < WYMIAR; i++) {
    int k = i;
#pragma omp parallel for ordered firstprivate(k)
    for (j = 0; j < WYMIAR; j++) {
#pragma omp atomic
      suma_parallel += a[i][j];
#pragma omp ordered
      {
        printf("wiersz %d, kolumna %d, wątek (%d, %d)\n", i, j,
               omp_get_thread_num(), k);
      }
    }
  }

  printf("Suma wyrazów tablicy równolegle (blokowo): %lf\n", suma_parallel);

  return 0;
}
