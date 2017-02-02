#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define WYMIAR 10000
#define ROZMIAR WYMIAR *WYMIAR

void mat_vec_row(double *a, double *x, double *y, int n);
void mat_vec_col(double *a, double *x, double *y, int n);

int main() {

  static double a[ROZMIAR], aa[ROZMIAR], x[WYMIAR], y[WYMIAR], z[WYMIAR];
  int n, i, j;
  const int ione = 1;
  const double done = 1.0;
  const double dzero = 0.0;
  double t1, t2;

  for (i = 0; i < ROZMIAR; i++)
    a[i] = 1.0001 * i;
  for (i = 0; i < ROZMIAR; i++)
    aa[i] = 1.0001 * i;
  for (i = 0; i < WYMIAR; i++)
    x[i] = 1.0 * (WYMIAR - i);

  n = WYMIAR;
  double nr_oper = (n * (n * 2.0));

  printf("ROW MAJOR\n");

  t1 = omp_get_wtime();
  mat_vec_row(a, x, y, n);
  t1 = omp_get_wtime() - t1;
  printf("time for one multiplication: %lf, Gflop/s: %lf, GB/s> %lf\n", t1,
         2.0e-9 * ROZMIAR / t1, 8.0e-9 * ROZMIAR / t1);

  // test
  printf("TEST ROW MAJOR\n");

  double sum_local_row;

  t1 = omp_get_wtime();
  for (i = 0; i < n; i++) {
    sum_local_row = 0.0;
#pragma omp parallel for default(none) private(j) firstprivate(i)              \
    shared(a, x, n) reduction(+ : sum_local_row)
    for (j = 0; j < n; j++) {
      sum_local_row += a[n * i + j] * x[j];
    }
    z[i] = sum_local_row;
  }
  t1 = omp_get_wtime() - t1;
  printf("time for one multiplication: %lf, Gflop/s: %lf, GB/s> %lf\n", t1,
         2.0e-9 * ROZMIAR / t1, 8.0e-9 * ROZMIAR / t1);

  printf("\nczas dostepu min %lf, max %lf\n", 1024 * 1024 * 1024 * t1 / nr_oper,
         1024 * 1024 * 1024 * t1 / (nr_oper / 2));
  printf("GFLOPS = %lf, GB/s - min = %lf, max = %lf\n\n",
         nr_oper / t1 / 1024 / 1024 / 1024,
         4 * nr_oper / t1 / 1024 / 1024 / 1024,
         8 * nr_oper / t1 / 1024 / 1024 / 1024);

  for (i = 0; i < WYMIAR; i++) {
    if (fabs(y[i] - z[i]) > 1.e-9 * z[i])
      printf("Blad!\n");
  }

  printf("COLUMN MAJOR\n");

  for (i = 0; i < n; i++)
    y[i] = 0.0;
  t1 = omp_get_wtime();
  mat_vec_col(a, x, y, n);
  t1 = omp_get_wtime() - t1;
  printf("time for one multiplication: %lf, Gflop/s: %lf, GB/s> %lf\n", t1,
         2.0e-9 * ROZMIAR / t1, 8.0e-9 * ROZMIAR / t1);

  // test
  printf("TEST COLUMN MAJOR\n");

  double sum_local_column;

  t1 = omp_get_wtime();
  for (i = 0; i < n; i++)
    z[i] = 0.0;

  for (i = 0; i < n; i++) {
#pragma omp parallel for default(none) private(j) firstprivate(i) shared(z, a, \
                                                                         x, n)
    for (j = 0; j < n; j++) {
      z[j] += a[n * j + i] * x[i];
    }
  }
  t1 = omp_get_wtime() - t1;
  printf("time for one multiplication: %lf, Gflop/s: %lf, GB/s> %lf\n", t1,
         2.0e-9 * ROZMIAR / t1, 8.0e-9 * ROZMIAR / t1);

  for (i = 0; i < WYMIAR; i++) {
    if (fabs(y[i] - z[i]) > 1.e-9 * z[i])
      printf("Blad!\n");
  }

  return 0;
}
