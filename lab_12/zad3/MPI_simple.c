#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "mpi.h"

struct Student {
  char name[15];
  int nr;
  double PR_mark;
  int big_data[2];
};

int main(int argc, char **argv) {

  int rank, size, source, dest, tag, i, packet_size;
  MPI_Status status;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (size > 1) {
    tag = 0;
    void *buffor;
    void *bufforOut;

    struct Student student;
    strncpy(student.name, "Norbert Małecki", 15);
    student.nr = 278974;
    student.PR_mark = 5.0;
    student.big_data[0] = 1;
    student.big_data[1] = 2;

    size = 15 * sizeof(char);
    MPI_Pack_size(15, MPI_CHAR, MPI_COMM_WORLD, &size);
    packet_size += size;

    size = sizeof(int);
    MPI_Pack_size(1, MPI_INTEGER, MPI_COMM_WORLD, &size);
    packet_size += size;

    size = sizeof(double);
    MPI_Pack_size(1, MPI_DOUBLE, MPI_COMM_WORLD, &size);
    packet_size += size;

    size = 2 * sizeof(int);
    MPI_Pack_size(2, MPI_INTEGER, MPI_COMM_WORLD, &size);
    packet_size += size;

    buffor = (void *)malloc(packet_size);
    bufforOut = (void *)malloc(packet_size);

    MPI_Pack(&student.name, 15, MPI_CHAR, &buffor, packet_size, &tag,
             MPI_COMM_WORLD);
    MPI_Pack(&student.nr, 1, MPI_INTEGER, &buffor, packet_size, &tag,
             MPI_COMM_WORLD);
    MPI_Pack(&student.PR_mark, 1, MPI_DOUBLE, &buffor, packet_size, &tag,
             MPI_COMM_WORLD);
    MPI_Pack(&student.big_data, 2, MPI_INTEGER, &buffor, packet_size, &tag,
             MPI_COMM_WORLD);

    if (rank == 0) {
      printf("Rozmiar pakietu: %d\n", packet_size);
      MPI_Send(&buffor, 1, MPI_PACKED, rank + 1, tag, MPI_COMM_WORLD);
      printf("Proces %d wysłał tablicę znaków %s do procesu %d\n", rank,
             student.name, rank + 1);
      // MPI_Recv(&datasent, 1024, MPI_CHAR, size - 1, tag, MPI_COMM_WORLD,
      // &status);
      // printf("Proces %d odebrał tablicę znaków %s od procesu %d\n", rank,
      // datasent, size);
    } else {
      MPI_Recv(&bufforOut, 1, MPI_PACKED, rank - 1, MPI_ANY_TAG, MPI_COMM_WORLD,
               &status);
      struct Student student2;
      MPI_Unpack(&bufforOut, packet_size, &tag, &student2.name, 15, MPI_CHAR,
                 MPI_COMM_WORLD);
      MPI_Unpack(&bufforOut, packet_size, &tag, &student2.nr, 1, MPI_INTEGER,
                 MPI_COMM_WORLD);
      MPI_Unpack(&bufforOut, packet_size, &tag, &student2.PR_mark, 1,
                 MPI_DOUBLE, MPI_COMM_WORLD);
      MPI_Unpack(&bufforOut, packet_size, &tag, &student2.big_data, 2,
                 MPI_INTEGER, MPI_COMM_WORLD);
      printf("Proces %d odebrał tablicę znaków %s od procesu %d\n", rank,
             student2.name, rank - 1);
      if (rank + 1 < size) {
        // MPI_Send(&bufforOut, 1, MPI_PACKED, rank + 1, tag, MPI_COMM_WORLD);
        // printf("Proces %d wysłał tablicę znaków %s do procesu %d\n", rank,
        // student2.name, rank + 1);
      }
    }
  } else {
    printf("Pojedynczy proces o randze: %d (brak komunikatów)\n", rank);
  }

  MPI_Finalize();

  return (0);
}
