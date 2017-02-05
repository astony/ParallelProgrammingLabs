#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define LICZBA_W 4
#define ROZMIAR 10000

double calka_omp(double a, double b);

int main(int argc, char *argv[]) {
  double p = 0.0;
  double k = 1.0;
  double result = 0.0;
  omp_set_num_threads(4);
#pragma omp parallel default(none) firstprivate(p, k) shared(result)
  {
#pragma omp single
    {
      long num_tasks = omp_get_num_threads() + 1;
      double n_loc = (k - p) / num_tasks;

      long itask;
      for (itask = 0; itask < num_tasks; itask++) {
        double p_task = p + itask * n_loc;
        if (p_task > k) {
          printf("Error in task decomposition! Exiting.\n");
          exit(0);
        }
        double k_task = p + (itask + 1) * n_loc;
        if (itask == num_tasks - 1)
          k_task = k;

        printf("From %lf to %lf\n", p_task, k_task);

#pragma omp task default(none) shared(result) firstprivate(p_task, k_task)
        {
          double a_sum_local = calka_omp(p_task, k_task);

#pragma omp critical
          result += a_sum_local;
        }
      }
    }
  }
  printf("Wynik rownolegle: %.10lf\n", result);
  printf("Wynik sekwencyjnie: %.10lf\n", calka_omp(p, k));
  return 0;
}

double f(double x) { return 1 / (1 + x * x); }

double calka_omp(double a, double b) {
  printf("Thread %d\n", omp_get_thread_num());
  double x1, calka, dx;
  int i;

  if (a > b) {
    dx = b;
    b = a;
    a = dx;
  }
  if (a == b)
    return 0;

  x1 = a;
  calka = 0.0;
  dx = (b - a) / (double)ROZMIAR;

  for (i = 1; i < ROZMIAR; ++i)
    calka += f(a + (i * dx));

  calka += (f(a) + f(b)) / 2;
  calka *= dx;

  return calka;
}
