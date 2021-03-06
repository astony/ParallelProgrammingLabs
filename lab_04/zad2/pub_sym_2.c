#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct struktura_t {
  int l_kf;
  pthread_mutex_t *tab_kuf;
  int l_kr;
  pthread_mutex_t *tab_kran;
};

struct struktura_t pub_wsk;

void *watek_klient(void *arg);
void printf_piwo(int moj_id, int kran);
int lock_kufel(int moj_id);
int lock_kran(int moj_id);

int main() {
  pthread_t *tab_klient;
  int *tab_klient_id;

  int l_kl, l_kf, l_kr, i;

  printf("\nLiczba klientow: ");
  scanf("%d", &l_kl);
  printf("\nLiczba kufli: ");
  scanf("%d", &l_kf);
  printf("\nLiczba kranow: ");
  scanf("%d", &l_kr);

  pub_wsk.l_kf = l_kf;
  pub_wsk.l_kr = l_kr;

  tab_klient = (pthread_t *)malloc(l_kl * sizeof(pthread_t));
  tab_klient_id = (int *)malloc(l_kl * sizeof(int));
  for (i = 0; i < l_kl; i++)
    tab_klient_id[i] = i;

  pub_wsk.tab_kuf = (pthread_mutex_t *)malloc(l_kf * sizeof(pthread_mutex_t));
  for (i = 0; i < l_kf; i++)
    pthread_mutex_init(&pub_wsk.tab_kuf[i], NULL);

  pub_wsk.tab_kran = (pthread_mutex_t *)malloc(l_kr * sizeof(pthread_mutex_t));
  for (i = 0; i < l_kr; i++)
    pthread_mutex_init(&pub_wsk.tab_kran[i], NULL);

  printf("\nOtwieramy pub (simple)!\n");
  printf("\nLiczba wolnych kufli %d\n", l_kf);

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

  int i, kran, kufel, result;

  int ile_musze_wypic = 2;

  printf("\nKlient %d, wchodzę do pubu\n", moj_id);

  for (i = 0; i < ile_musze_wypic; i++) {

    kufel = lock_kufel(moj_id);
    kran = lock_kran(moj_id);

    printf("\nKlient %d, zwalniam kran %d\n", moj_id, kran);
    pthread_mutex_unlock(&pub_wsk.tab_kran[kran]);

    printf_piwo(moj_id, kran);

    printf("\nKlient %d, odkladam kufel %d\n", moj_id, kufel);
    pthread_mutex_unlock(&pub_wsk.tab_kuf[kufel]);
  }

  return (NULL);
}

void printf_piwo(moj_id, kran) {
  if (kran == 0)
    printf("\nKlient %d, pije piwo Guinness\n", moj_id);
  else if (kran == 1)
    printf("\nKlient %d, pije piwo Żywiec\n", moj_id);
  else if (kran == 2)
    printf("\nKlient %d, pije piwo Heineken\n", moj_id);
  else if (kran == 3)
    printf("\nKlient %d, pije piwo Okocim\n", moj_id);
  else if (kran == 4)
    printf("\nKlient %d, pije piwo Carlsberg\n", moj_id);
  else
    printf("\nKlient %d, pije piwo nieznane", moj_id);
  sleep(1);
}

int lock_kufel(moj_id) {
  int i, result;
  result = 0;
  do {
    for (i = 0; i < pub_wsk.l_kf; ++i)
      if (pthread_mutex_trylock(&pub_wsk.tab_kuf[i]) == 0) {
        printf("\nKlient %d, wybrałem kufel %d\n", moj_id, i);
        return i;
      }
  } while (result == 0);
  return -1;
}

int lock_kran(moj_id) {
  int j, result;
  result = 0;
  do {
    for (j = 0; j < pub_wsk.l_kr; ++j)
      if (pthread_mutex_trylock(&pub_wsk.tab_kran[j]) == 0) {
        printf("\nKlient %d, nalewam z kranu %d\n", moj_id, j);
        sleep(4);
        return j;
      }
  } while (result == 0);
  return -1;
}
