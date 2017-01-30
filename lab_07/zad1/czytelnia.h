#ifndef _czytelnia_
#define _czytelnia_

/*** Definicje typow zmiennych ***/
typedef struct {
  // <- zasoby czytelni
  int liczba_pisz;
  int kolejka_pis;
  pthread_mutex_t kolejka_mut_pis;
  pthread_cond_t pisarze;
  pthread_mutex_t mutex_czytelnia;
  pthread_mutex_t liczba_pisz_mut; // Mutex ktory chroni przed nie wiejściem do
                                   // ifa (kolejki) kiedy się powinno
  // Do tej pory występowało wszystko potrzebne dla współpracy pisarzy
  int liczba_czyt;
  int kolejka_czyt;
  pthread_mutex_t kolejka_mut_czyt;
  pthread_mutex_t liczba_czyt_mut;
  pthread_cond_t czytelnicy;
} czytelnia_t;

/*** Deklaracje procedur interfejsu ***/
void inicjuj(czytelnia_t *czytelnia_p);
void czytam(czytelnia_t *czytelnia_p, int id);
void pisze(czytelnia_t *czytelnia_p, int id);

int my_read_lock_lock(czytelnia_t *czytelnia_p, int id);
int my_read_lock_unlock(czytelnia_t *czytelnia_p, int id);
int my_write_lock_lock(czytelnia_t *czytelnia_p, int id);
int my_write_lock_unlock(czytelnia_t *czytelnia_p, int id);

#endif
