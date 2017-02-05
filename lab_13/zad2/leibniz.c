#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mpi.h"

int main(int argc, char *argv[]) {
  int id, p, liczba, podzial;

  double wynik;

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &id);

  MPI_Comm_size(MPI_COMM_WORLD, &p);

  MPI_Status status;

  if (id == 0) {
    int liczba = atoi(argv[1]);
    int podzial = (int)(liczba / p + 1);
    printf("Liczba %d, Podzial %d\n", liczba, podzial);
  }

  MPI_Bcast(&liczba, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);
  MPI_Bcast(&podzial, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

  printf("Liczba %d, Podzial %d\n", liczba, podzial);

  int pocz = (int)(id + 1);
  int kon = (int)(id * podzial);

  printf("Poczatek %d, Koniec %d\n", pocz, kon);

  int i;
  int suma_cz;
  for (i = pocz; i < kon; i += 2) {
    suma_cz += (1 / 2 * i + 1);
    suma_cz -= (1 / 2 * (i + 1) + 1);
  }

  MPI_Reduce(&suma_cz, &wynik, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

  if (id == 0)
    printf("Wynik obliczeń %lf\n", wynik);

  MPI_Finalize();

  return;
}
