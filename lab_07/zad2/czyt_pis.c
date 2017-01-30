#include "czytelnia.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *funkcja_czytelnika(void *);
void *funkcja_pisarza(void *);
czytelnia_t czytelnia;

int main() {
  inicjuj(&czytelnia); // Inicjalizacja czytelni
  int i;
  const int n_pis = 5;
  const int n_czy = 10;
  int id_pis[n_pis];
  int id_czy[n_czy];
  pthread_t pisarze[n_pis];
  pthread_t czytelnicy[n_czy];

  for (i = 0; i < n_pis; i++) {
    id_pis[i] = i + 1;
    pthread_create(&pisarze[i], NULL, funkcja_pisarza, (void *)&id_pis[i]);
  }
  for (i = 0; i < n_czy; i++) {
    id_czy[i] = i + 1;
    pthread_create(&czytelnicy[i], NULL, funkcja_czytelnika,
                   (void *)&id_czy[i]);
  }
  for (i = 0; i < n_pis; i++)
    pthread_join(pisarze[i], NULL);
  for (i = 0; i < n_czy; i++)
    pthread_join(czytelnicy[i], NULL);
}

void *funkcja_czytelnika(void *arg) {
  int czas_oczekiwania;
  int id = *((int *)arg);
  for (;;) {
    czas_oczekiwania = rand() % 10000000;
    printf("Czytelnik %d czeka %d .\n", id, czas_oczekiwania);
    usleep(czas_oczekiwania);
    // printf("czytelnik %d - przed zamkiem\n", pthread_self());
    my_read_lock_lock(&czytelnia, id);
    // printf("czytelnik %d - wchodze\n", pthread_self());
    czytam(&czytelnia, id);
    // printf("czytelnik %d - wychodze\n", pthread_self());
    my_read_lock_unlock(&czytelnia, id);
  }
}

void *funkcja_pisarza(void *arg) {
  int czas_oczekiwania;
  int id = *((int *)arg);
  for (;;) {
    czas_oczekiwania = rand() % 11000000;
    printf("Pisarz %d czeka %d aż coś wymyśli.\n", id, czas_oczekiwania);
    usleep(czas_oczekiwania);
    printf("Pisarz %d wymyślił.\n", id);
    // printf("pisarz %d - przed zamkiem\n", pthread_self());
    my_write_lock_lock(&czytelnia, id);
    // korzystanie z zasobow czytelni
    // printf("pisarz %d - wchodze\n", pthread_self());
    pisze(&czytelnia, id);
    // printf("pisarz %d - wychodze\n", pthread_self());
    my_write_lock_unlock(&czytelnia, id);
    // printf("pisarz %d - po zamku\n", pthread_self());
  }
}
