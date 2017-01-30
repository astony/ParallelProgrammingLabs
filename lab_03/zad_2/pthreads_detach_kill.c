#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int zmienna_wspolna = 0;

#define WYMIAR 1000
#define ROZMIAR WYMIAR *WYMIAR

double a[ROZMIAR], b[ROZMIAR], c[ROZMIAR];

struct own_id {
  int a;
} own_id;

double czasozajmowacz() {
  int i, j, k;
  int n = WYMIAR;
  for (i = 0; i < ROZMIAR; i++)
    a[i] = 1.0 * i;

  for (i = 0; i < ROZMIAR; i++)
    b[i] = 1.0 * (ROZMIAR - i);

  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++) {
      c[i + n * j] = 0.0;
      for (k = 0; k < n; k++)
        c[i + n * j] += a[i + n * k] * b[k + n * j];
    }

  return (c[ROZMIAR - 1]);
}

void *wyswietl_id(void *arg_wsk) {
  int id = *((int *)arg_wsk);

  printf("\twatek potomny: Pid: %lu, id: %d\n", pthread_self(), id);

  sleep(2);
}

void *zadanie_watku(void *arg_wsk) {
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
  printf("\twatek potomny: uniemozliwione zabicie\n");

  czasozajmowacz();

  printf("\twatek potomny: umozliwienie zabicia\n");
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

  pthread_testcancel();

  zmienna_wspolna++;
  printf("\twatek potomny: zmiana wartosci zmiennej wspolnej\n");

  return (NULL);
}

int main() {
  pthread_t tid1, tid2, tid3;
  pthread_t tid[100];
  pthread_attr_t attr;
  void *wynik;
  int i;

  for (i = 0; i < 100; i++) {
    int *id;
    id = malloc(sizeof *id);
    if (!id) {
      perror("malloc");
      return EXIT_FAILURE;
    }
    id[0] = i;
    pthread_create(&tid[i], NULL, wyswietl_id, &id[0]);
  }

  for (i = 0; i < 100; i++) {
    printf("\twatek glowny: wyslanie sygnalu zabicia watku nr %d\n", i);
    pthread_cancel(tid[i]);

    pthread_join(tid[i], &wynik);

    // Co nalezy zrobić przed sprawdzeniem czy wątki się skonczyły?
    if (wynik == PTHREAD_CANCELED)
      printf("\twatek glowny: watek potomny zostal zabity\n");
    else
      printf("\twatek glowny: watek potomny NIE zostal zabity - blad\n");
  }

  pthread_exit(NULL);
}
