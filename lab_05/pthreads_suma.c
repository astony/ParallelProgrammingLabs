#include <omp.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#include "pomiar_czasu.h"

#define ROZMIAR 100010003
#define LICZBA_W 8
#define PI 3.14159265358979323846

pthread_mutex_t muteks;
pthread_t watki[LICZBA_W];

double *tab;
double suma = 0;
double global_array_of_local_sums[LICZBA_W];

typedef struct boundaries {
  double a;
  double b;
  int iter;
  int id;
} boundaries;

double calka_sekwencyjnie(double a, double b);

double calka_pthread_mutex(double a, double b);
void *pthread_range_mutex(void *wsk);

double calka_pthread_local_sum(double a, double b);
void *pthread_range_local_sum(void *wsk);

double calka_omp(double a, double b);

int main(int argc, char *argv[]) {

  int i;

  double t1, t2, t3, t4;

  int indeksy[LICZBA_W];
  for (i = 0; i < LICZBA_W; ++i)
    indeksy[i] = i;

  tab = (double *)malloc(ROZMIAR * sizeof(double));
  for (i = 0; i < ROZMIAR; ++i)
    tab[i] = ((double)i + 1) / ROZMIAR;

  pthread_mutex_init(&muteks, NULL);

  // Całka sekwencyjnie
  t1 = czas_zegara();
  printf("Sekwencyjna:\n");
  double result1 = calka_sekwencyjnie(0.0, 1.0);
  t1 = czas_zegara() - t1;
  printf("\tCałka wynosi: %1.20lf.\n", result1);
  printf("\tBłąd: %1.20lf\n", PI - result1);
  printf("\tCzas obliczeń: %f\n\n", t1);

  // Całka Pthread mutex
  printf("Pthread mutex:\n");
  t2 = czas_zegara();
  double result2 = calka_pthread_mutex(0.0, 1.0);
  t2 = czas_zegara() - t2;
  printf("\tCałka wynosi: %1.20lf.\n", result2);
  printf("\tBłąd: %1.20lf\n", 2.0 - result2);
  printf("\tCzas obliczen = %lf\n\n", t2);

  // Całka Pthread lokalne sumy (wyeliminowanie mutexów)
  t3 = czas_zegara();
  printf("Pthread lokalne sumy:\n");
  double result3 = calka_pthread_local_sum(0.0, 1.0);
  t3 = czas_zegara() - t3;
  printf("\tCałka wynosi: %1.20lf.\n", result3);
  printf("\tBłąd: %1.20lf\n", PI - result3);
  printf("\tCzas obliczen = %lf\n\n", t3);

  // Całka OMP pętla równoległa
  t4 = czas_zegara();
  printf("OMP pętla równoległa:\n");
  double result4 = calka_omp(0.0, 1.0);
  t4 = czas_zegara() - t4;
  printf("\tCałka wynosi: %1.20lf.\n", result4);
  printf("\tBłąd: %1.20lf\n", PI - result4);
  printf("\tCzas obliczen = %lf\n\n", t4);

  return 0;
}

double f(double x) { return 1 / (1 + x * x); }

double calka_sekwencyjnie(double a, double b) {
  printf("\tLiczba przedziałów na dla wątku nr %d: %d\n", 0, ROZMIAR);
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

double calka_pthread_mutex(double a, double b) {
  suma = 0;
  unsigned long int i;
  double divisor = (b - a) / LICZBA_W;
  boundaries bnd[LICZBA_W];

  int iter = ROZMIAR / LICZBA_W;

  for (i = 0; i < LICZBA_W - 1; ++i) {
    bnd[i].a = a + i * divisor;
    bnd[i].b = a + (i + 1) * divisor;
    bnd[i].iter = iter;
    printf("\tLiczba przedziałów dla wątku nr %d: %d\n", i, bnd[i].iter);
    bnd[i].id = i;
    pthread_create(&watki[i], NULL, pthread_range_mutex, &(bnd[i]));
  }

  bnd[i].a = a + i * divisor;
  bnd[i].b = b;
  bnd[i].iter = ROZMIAR - (iter * i);
  printf("\tLiczba przedziałów dla wątku nr %d: %d\n", i, bnd[i].iter);
  bnd[i].id = i;
  pthread_create(&watki[i], NULL, pthread_range_mutex, &(bnd[i]));

  for (i = 0; i < LICZBA_W; ++i)
    pthread_join(watki[i], NULL);

  return suma;
}

void *pthread_range_mutex(void *wsk) {
  boundaries *bnd = (boundaries *)wsk;

  double dx = (bnd->b - bnd->a) / bnd->iter;

  double result = 0;
  int i;
  for (i = 1; i < bnd->iter; i++) {
    result += f(bnd->a + (i * dx)); // liczym sume elementów od a+dx do b-d;
  }
  result += (f(bnd->a) + f(bnd->b)) / 2;
  result *= dx;

  pthread_mutex_lock(&muteks);
  suma += result;
  pthread_mutex_unlock(&muteks);

  pthread_exit((void *)0);
}

double calka_pthread_local_sum(double a, double b) {
  suma = 0;
  unsigned long int i;
  int iter = ROZMIAR / LICZBA_W;
  double divisor = (b - a) / LICZBA_W;
  boundaries bnd[LICZBA_W];

  for (i = 0; i < LICZBA_W - 1; ++i) {
    bnd[i].a = a + i * divisor;
    bnd[i].b = a + (i + 1) * divisor;
    bnd[i].iter = iter;
    printf("\tLiczba przedziałów dla wątku nr %d: %d\n", i, bnd[i].iter);
    bnd[i].id = i;
    pthread_create(&watki[i], NULL, pthread_range_mutex, &(bnd[i]));
  }

  bnd[i].a = a + i * divisor;
  bnd[i].b = b;
  bnd[i].iter = ROZMIAR - (iter * i);
  printf("\tLiczba przedziałów dla wątku nr %d: %d\n", i, bnd[i].iter);
  bnd[i].id = i;
  pthread_create(&watki[i], NULL, pthread_range_mutex, &(bnd[i]));

  for (i = 0; i < LICZBA_W; ++i)
    pthread_join(watki[i], NULL);

  for (i = 0; i < LICZBA_W; ++i)
    suma += global_array_of_local_sums[i];

  return suma;
}

void *pthread_range_local_sum(void *wsk) {
  boundaries *bnd = (boundaries *)wsk;

  double dx = (bnd->b - bnd->a) / bnd->iter;

  double result = 0;
  int i;
  for (i = 1; i < bnd->iter; i++) {
    result += f(bnd->a + (i * dx)); // liczym sume elementów od a+dx do b-d;
  }
  result += (f(bnd->a) + f(bnd->b)) / 2;
  result *= dx;

  global_array_of_local_sums[bnd->id] = result;

  pthread_exit((void *)0);
}

double calka_omp(double a, double b) {
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

#pragma omp parallel for num_threads(LICZBA_W) default(none)                   \
    shared(i, dx, a) reduction(+ : calka)
  for (i = 1; i < ROZMIAR; ++i)
    calka += f(a + (i * dx));

  calka += (f(a) + f(b)) / 2;
  calka *= dx;

  return calka;
}
