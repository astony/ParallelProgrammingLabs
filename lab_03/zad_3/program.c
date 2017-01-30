#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

struct struktura_t {
  int a;
  double b;
  char c;
};
void *zadanie_watku(void *arg_wsk);

int main() {
  pthread_t tid1, tid2;
  struct struktura_t struktura_main = {1, 3.14, 'c'};

  printf("Struktura main wartosci przed tworzeniem wątków: a = %d, b = %lf, c "
         "= %c\n",
         struktura_main.a, struktura_main.b, struktura_main.c);

  pthread_create(&tid1, NULL, zadanie_watku, &struktura_main);
  pthread_create(&tid2, NULL, zadanie_watku, &struktura_main);
  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  printf("Struktura main wartosci po zakończeniu wątków: a = %d, b = %lf, c = "
         "%c\n",
         struktura_main.a, struktura_main.b, struktura_main.c);

  pthread_exit(NULL);
}

void *zadanie_watku(void *arg_wsk) {
  struct struktura_t *wskaznik_do_struktury_main = arg_wsk;
  wskaznik_do_struktury_main->a = 3;

  struct struktura_t struktura_lokalna;
  struktura_lokalna = *((struct struktura_t *)arg_wsk);
  struktura_lokalna.a = 5;

  printf("Dostep do wartosci z procedury main: a = %d, b = %lf, c = %c\n",
         wskaznik_do_struktury_main->a, wskaznik_do_struktury_main->b,
         wskaznik_do_struktury_main->c);

  wskaznik_do_struktury_main->b = 3.26;

  printf("Dostep do skopiowanych lokalnych wartosci: a = %d, b = %lf, c = %c\n",
         struktura_lokalna.a, struktura_lokalna.b, struktura_lokalna.c);

  return (NULL);
}
