#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

int main(int argc, char *argv[]) {

  int rank, size, source, dest, tag, i;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size > 1) {
    char datasent[1024];
    tag = 0;
    if (rank == 0) {
      char hostname[1024];
      gethostname(hostname, 1024);
      MPI_Send(&hostname, 1024, MPI_CHAR, rank + 1, tag, MPI_COMM_WORLD);
      printf("Proces %d wysłał tablicę znaków %s do procesu %d\n", rank,
             hostname, rank + 1);
      MPI_Recv(&datasent, 1024, MPI_CHAR, size - 1, tag, MPI_COMM_WORLD,
               &status);
      printf("Proces %d odebrał tablicę znaków %s od procesu %d\n", rank,
             datasent, size);
    } else {
      MPI_Recv(&datasent, 1024, MPI_CHAR, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD,
               &status);
      printf("Proces %d odebrał tablicę znaków %s od procesu %d\n", rank,
             datasent, rank - 1);
      if (rank + 1 < size) {
        MPI_Send(&datasent, 1024, MPI_CHAR, rank + 1, tag, MPI_COMM_WORLD);
        printf("Proces %d wysłał tablicę znaków %s do procesu %d\n", rank,
               datasent, rank + 1);
      } else {
        MPI_Send(&datasent, 1024, MPI_CHAR, 0, tag, MPI_COMM_WORLD);
        printf("Proces %d wysłał tablicę znaków %s do procesu %d\n", rank,
               datasent, 0);
      }
    }
  } else {
    printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }

  MPI_Finalize();

  return (0);
}
