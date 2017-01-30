#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int l_kl, l_kf, l_kr;
pthread_mutex_t mutex1;
pthread_mutex_t mutex2;

void *watek_klient(void *arg);

int main() {

  pthread_t *tab_klient;
  int *tab_klient_id;

  int i;

  printf("\nLiczba klientow: ");
  scanf("%d", &l_kl);

  printf("\nLiczba kufli: ");
  scanf("%d", &l_kf);

  // printf("\nLiczba kranow: "); scanf("%d", &l_kr);
  l_kr = 1;

  tab_klient = (pthread_t *)malloc(l_kl * sizeof(pthread_t));
  tab_klient_id = (int *)malloc(l_kl * sizeof(int));
  for (i = 0; i < l_kl; i++)
    tab_klient_id[i] = i;

  printf("\nOtwieramy pub (simple)!\n");
  printf("\nLiczba wolnych kufli %d\n", l_kf);

  pthread_mutex_init(&mutex1, NULL);
  pthread_mutex_init(&mutex2, NULL);
  for (i = 0; i < l_kl; i++) {
    pthread_create(&tab_klient[i], NULL, watek_klient, &tab_klient_id[i]);
  }
  for (i = 0; i < l_kl; i++) {
    pthread_join(tab_klient[i], NULL);
  }
  printf("\nZamykamy pub!\n");

  return 0;
}

void *watek_klient(void *arg_wsk) {

  int moj_id = *((int *)arg_wsk);

  int i, j, kufel, result;
  int ile_musze_wypic = 2;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id);

  for (i = 0; i < ile_musze_wypic; i++) {

    result = 0;

    do {

      pthread_mutex_lock(&mutex1);
      if (l_kf > 0) {
        printf("\nKlient %d, wybieram kufel\n", moj_id);
        printf("\nKlient %d, wybrałem kufel %d\n", moj_id, l_kf);
        l_kf--;
        result = 1;
      }
      pthread_mutex_unlock(&mutex1);
      if (result == 1) {

        j = 0;
        printf("\nKlient %d, nalewam z kranu %d\n", moj_id, j);
        usleep(300);

        printf("\nKlient %d, pije\n", moj_id);
        nanosleep((struct timespec[]){{0, 500000000L}}, NULL);

        pthread_mutex_lock(&mutex2);
        l_kf++;
        printf("\nKlient %d, odkladam kufel %d\n", moj_id, l_kf);
        pthread_mutex_unlock(&mutex2);

        result = 1;
      }
    } while (result == 0);
  }

  printf("\nKlient %d, wychodzę z pubu\n", moj_id);

  return (NULL);
}
