#include "czytelnia.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*** Implementacja procedur interfejsu ***/
int my_read_lock_lock(czytelnia_t *czytelnia_p, int id) {
  pthread_mutex_lock(&czytelnia_p->liczba_pisz_mut); // Nie chce żeby mi ktoś go
  // nadpisał wiec write lock
  pthread_mutex_lock(
      &czytelnia_p->kolejka_mut_czyt); // Nie chce ani read , ani write
  int odblokowalem_w_petli = 0;
  if (czytelnia_p->liczba_pisz > 0 || czytelnia_p->kolejka_czyt > 0) {
    pthread_mutex_unlock(&czytelnia_p->liczba_pisz_mut);
    odblokowalem_w_petli = 1;
    printf(
        "Czytelnik %d ustawił się w kolejce do czytania! Jest %d w kolejce.\n",
        id, czytelnia_p->kolejka_czyt + 1);
    czytelnia_p->kolejka_czyt++;
    pthread_mutex_unlock(&czytelnia_p->kolejka_mut_czyt);
    pthread_cond_wait(&czytelnia_p->czytelnicy, &czytelnia_p->mutex_czytelnia);
    pthread_mutex_lock(&czytelnia_p->kolejka_mut_czyt);
    czytelnia_p->kolejka_czyt--;
    pthread_mutex_unlock(&czytelnia_p->kolejka_mut_czyt);
  }
  if (odblokowalem_w_petli == 0) {
    pthread_mutex_unlock(&czytelnia_p->liczba_pisz_mut);
    pthread_mutex_unlock(&czytelnia_p->kolejka_mut_czyt);
  }
  pthread_mutex_lock(&czytelnia_p->liczba_czyt_mut);
  czytelnia_p->liczba_czyt++;
  pthread_mutex_unlock(&czytelnia_p->liczba_czyt_mut);
  pthread_cond_signal(&czytelnia_p->czytelnicy);
  return 0;
}
int my_read_lock_unlock(czytelnia_t *czytelnia_p, int id) {
  pthread_mutex_lock(&czytelnia_p->liczba_czyt_mut);
  czytelnia_p->liczba_czyt--;
  printf("Czytelnik %d wychodzi z czytelni. Pozostało %d czytelnikow\n", id,
         czytelnia_p->liczba_czyt);
  if (czytelnia_p->liczba_czyt == 0) {
    pthread_cond_signal(&czytelnia_p->pisarze);
  }
  pthread_mutex_unlock(&czytelnia_p->liczba_czyt_mut);
  return 0;
}
int my_write_lock_lock(czytelnia_t *czytelnia_p, int id) {
  pthread_mutex_lock(&czytelnia_p->liczba_pisz_mut);
  pthread_mutex_lock(&czytelnia_p->kolejka_mut_pis);
  pthread_mutex_lock(&czytelnia_p->liczba_czyt_mut);
  int odblokowalem_w_petli = 0;
  if (czytelnia_p->liczba_pisz > 0 || czytelnia_p->kolejka_pis > 0 ||
      czytelnia_p->liczba_czyt > 0) {
    pthread_mutex_unlock(&czytelnia_p->liczba_pisz_mut);
    pthread_mutex_unlock(&czytelnia_p->liczba_czyt_mut);
    odblokowalem_w_petli = 1;
    printf("Pisarz %d ustawił się w kolejce do pisania! Jest %d w kolejce.\n",
           id, czytelnia_p->kolejka_pis + 1);
    czytelnia_p->kolejka_pis++;
    pthread_mutex_unlock(&czytelnia_p->kolejka_mut_pis);
    pthread_cond_wait(&czytelnia_p->pisarze, &czytelnia_p->mutex_czytelnia);
    pthread_mutex_lock(&czytelnia_p->kolejka_mut_pis);
    czytelnia_p->kolejka_pis--;
    pthread_mutex_unlock(&czytelnia_p->kolejka_mut_pis);
  }
  if (odblokowalem_w_petli == 1)
    pthread_mutex_lock(&czytelnia_p->liczba_pisz_mut);
  if (odblokowalem_w_petli != 1) {
    pthread_mutex_unlock(&czytelnia_p->kolejka_mut_pis);
    pthread_mutex_unlock(&czytelnia_p->liczba_czyt_mut);
  }
  czytelnia_p->liczba_pisz++;
  pthread_mutex_unlock(&czytelnia_p->liczba_pisz_mut);
  return 0;
}
int my_write_lock_unlock(czytelnia_t *czytelnia_p, int id) {
  pthread_mutex_lock(&czytelnia_p->liczba_pisz_mut);
  printf("Pisarz %d skonczyl pisac. W czytelni pozostalo %d pisarzy\n", id,
         czytelnia_p->liczba_pisz - 1);
  czytelnia_p->liczba_pisz--; // pisarz wychodzi z czytelni
  pthread_mutex_unlock(&czytelnia_p->liczba_pisz_mut);
  pthread_mutex_lock(&czytelnia_p->kolejka_mut_czyt);
  if (czytelnia_p->kolejka_czyt > 0) {
    pthread_cond_signal(&czytelnia_p->czytelnicy);
  } else {
    pthread_cond_signal(&czytelnia_p->pisarze);
  }
  pthread_mutex_unlock(&czytelnia_p->kolejka_mut_czyt);
  return 0;
}

void inicjuj(czytelnia_t *czytelnia_p) {
  czytelnia_p->liczba_pisz = 0;
  czytelnia_p->kolejka_pis = 0;
  pthread_mutex_init(&czytelnia_p->kolejka_mut_pis, NULL);
  pthread_cond_init(&czytelnia_p->pisarze, NULL);
  pthread_mutex_init(&czytelnia_p->mutex_czytelnia, NULL);
  pthread_mutex_init(&czytelnia_p->liczba_pisz_mut, NULL);
  // Zmienne ustawione aby sami pisarze się dogadali
  czytelnia_p->liczba_czyt = 0;
  czytelnia_p->kolejka_czyt = 0;
  pthread_cond_init(&czytelnia_p->czytelnicy, NULL);
  pthread_mutex_init(&czytelnia_p->kolejka_mut_czyt, NULL);
  pthread_mutex_init(&czytelnia_p->liczba_czyt_mut, NULL);
}

void czytam(czytelnia_t *czytelnia_p, int id) {
  printf("Czytelnik %d czyta. W czytelni jest %d\n", id,
         czytelnia_p->liczba_czyt);
  usleep(rand() % 5000000);
}

void pisze(czytelnia_t *czytelnia_p, int id) {
  printf("Pisarz %d pisze. W czytelni jest %d\n", id, czytelnia_p->liczba_pisz);
  usleep(rand() % 5000000);
}
