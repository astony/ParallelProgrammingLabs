#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define LICZBA_W 4

pthread_t watki[LICZBA_W];
pthread_mutex_t muteks_bariera;
pthread_cond_t cond_bariera;
int licznik_bariera = LICZBA_W;

void *cokolwiek(void *arg);
void bariera();

int main(int argc, char *argv[]) {
  int i, indeksy[LICZBA_W];
  for (i = 0; i < LICZBA_W; i++)
    indeksy[i] = i;
  pthread_mutex_init(&muteks_bariera, NULL);
  pthread_cond_init(&cond_bariera, NULL);
  for (i = 0; i < LICZBA_W; i++)
    pthread_create(&watki[i], NULL, cokolwiek, (void *)&indeksy[i]);
  for (i = 0; i < LICZBA_W; i++)
    pthread_join(watki[i], NULL);
  pthread_exit(NULL);
}

void *cokolwiek(void *arg) {
  int i, moj_id;
  moj_id = *((int *)arg);
  printf("przed bariera 1 - watek %d\n", moj_id);
  bariera();
  printf("przed bariera 2 - watek %d\n", moj_id);
  bariera();
  printf("przed bariera 3 - watek %d\n", moj_id);
  bariera();
  printf("przed bariera 4 - watek %d\n", moj_id);
  bariera();
  pthread_exit((void *)0);
}

void bariera() {
  pthread_mutex_lock(&muteks_bariera);
  licznik_bariera--;
  if (licznik_bariera == 0) {
    pthread_cond_broadcast(&cond_bariera);
    licznik_bariera = LICZBA_W;
  } else {
    pthread_cond_wait(&cond_bariera, &muteks_bariera);
  }
  pthread_mutex_unlock(&muteks_bariera);
}