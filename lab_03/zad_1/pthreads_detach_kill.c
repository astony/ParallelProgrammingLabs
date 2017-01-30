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

void *zadanie_watku(void *arg_wsk) {
  struct own_id id = *((struct own_id *)arg_wsk);

  printf("Pid: %d, id: %d", pthread_self(), id.a);

  printf("\twatek potomny: uniemozliwione zabicie\n");
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

  czasozajmowacz();

  printf("\twatek potomny: umozliwienie zabicia\n");
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);

  pthread_testcancel();

  zmienna_wspolna++;
  printf("\twatek potomny: zmiana wartosci zmiennej wspolnej\n");

  return (NULL);
}

int main() {
  pthread_t tid;
  pthread_attr_t attr;
  void *wynik;
  int i;

  // Wątek przyłączalny

  printf("watek glowny: tworzenie watku potomnego nr 1\n");

  struct own_id id_1;
  id_1.a = 1;
  pthread_create(&tid, NULL, zadanie_watku, &id_1);

  sleep(2); // czas na uruchomienie watku

  printf("\twatek glowny: wyslanie sygnalu zabicia watku\n");
  pthread_cancel(tid);

  pthread_join(tid, &wynik);

  // Co nalezy zrobić przed sprawdzeniem czy wątki się skonczyły?
  if (wynik == PTHREAD_CANCELED)
    printf("\twatek glowny: watek potomny zostal zabity\n");
  else
    printf("\twatek glowny: watek potomny NIE zostal zabity - blad\n");

  // Odłączanie wątku

  zmienna_wspolna = 0;

  printf("watek glowny: tworzenie watku potomnego nr 2\n");

  struct own_id id_2;
  id_2.a = 2;
  pthread_create(&tid, NULL, zadanie_watku, &id_2);

  sleep(2); // czas na uruchomienie watku

  printf("\twatek glowny: odlaczenie watku potomnego\n");

  // Instrukcja odłączenia?
  pthread_detach(tid);

  printf("\twatek glowny: wyslanie sygnalu zabicia watku odlaczonego\n");
  pthread_cancel(tid);

  for (i = 0; i < 10; i++) {
    sleep(1);
    if (zmienna_wspolna != 0)
      break;
  }

  if (zmienna_wspolna == 0)
    printf("\twatek glowny: watek potomny PRAWDOPODOBNIE zostal zabity\n");
  else
    printf("\twatek glowny: watek potomny PRAWDOPODOBNIE NIE zostal zabity - "
           "blad\n");

  // Wątek odłączony

  // Inicjacja atrybutów?

  // Ustawianie typu watku na odłaczony

  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);

  printf("watek glowny: tworzenie odlaczonego watku potomnego nr 3\n");
  struct own_id id_3;
  id_3.a = 3;
  pthread_create(&tid, &attr, zadanie_watku, &id_3);
  pthread_attr_destroy(&attr);

  // Niszczenie atrybutów

  printf("\twatek glowny: koniec pracy, watek odlaczony pracuje dalej\n");

  pthread_exit(NULL); // co stanie sie gdy uzyjemy exit(0)?
                      // exit(0);
}
