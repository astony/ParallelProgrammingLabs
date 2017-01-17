#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sched.h>
#include <linux/sched.h>

#include "pomiar_czasu.h"

int zmienna_globalna=0;

#define ROZMIAR_STOSU 1024*64

int funkcja_watku( void *arg )
{
  int wynik;
  wynik = execv("./program", NULL);
  if(wynik == -1)
    printf("Proces potomny nie wykonal programu\n");
  return 0;
}

int main()
{

  void *stos, *stos2;
  pid_t pid_1, pid_2;
  
  stos = malloc( ROZMIAR_STOSU );
  stos2 = malloc( ROZMIAR_STOSU );
  if (stos == 0 || stos2 == 0)
  {
    printf("Proces nadrzędny - blad alokacji stosu\n");
    exit( 1 );
  }

  printf("Pid: %d\n\n", getpid());

  inicjuj_czas();
 
  pid_1 = clone( &funkcja_watku, (void *) stos + ROZMIAR_STOSU, 
		   CLONE_FS | CLONE_FILES | CLONE_SIGHAND, 
                   NULL );

  pid_2 = clone( &funkcja_watku, (void *) stos2 + ROZMIAR_STOSU, 
		   CLONE_FS | CLONE_FILES | CLONE_SIGHAND, 
                   NULL );

  waitpid(pid_1, NULL, __WCLONE);
  waitpid(pid_2, NULL, __WCLONE);

  drukuj_czas();

  printf("\nzmienna globalna = %d\n", zmienna_globalna);

  free( stos );
  free( stos2 );

  return 0;
}
