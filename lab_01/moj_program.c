#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "pomiar_czasu.h"
 
const int liczba = 100000;

int main()
{

  double a, b, c;
  int i,j,k,l;

  double t1, t2;

  k = -50000;

  inicjuj_czas();

  for(i = 0; i < liczba; ++i)
    printf("%d ",k + i);

  printf("\n\n");

  drukuj_czas();

  a = 1.000001;

  t1 = czas_zegara();
  t2 = czas_CPU(); 

  for(i = 0; i < liczba; ++i)
    a = 1.000001*a+0.000001; 

  t1 = czas_zegara() - t1;
  t2 = czas_CPU() - t2;

  printf("\n");
  printf("czas CPU         = %lf\n", t1);
  printf("czas zegarowy    = %lf\n", t2);

  printf("\nWynik operacji arytmetycznych: %lf\n", a);

  return 0;
}
