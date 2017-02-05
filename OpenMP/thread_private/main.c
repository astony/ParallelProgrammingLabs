#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

static int f_threadprivate = 7;
#pragma omp threadprivate(f_threadprivate)

int main() {

  omp_set_num_threads(2);

  printf("\nprzed rozpoczeciem obszaru rownoleglego:\n");
  printf("\tf_threadprivate \t= %d\n", f_threadprivate);

#pragma omp parallel
#pragma omp critical
  {
    printf("\nw 1 obszarze równoległym: aktualna liczba watkow %d, moj ID %d\n",
           omp_get_num_threads(), omp_get_thread_num());
    printf("\n\tprzed przypisaniem:\n");
    printf("\t\tf_threadprivate \t= %d\n", f_threadprivate);
    f_threadprivate = omp_get_thread_num();
    printf("\n\tpo przypisaniu:\n");
    printf("\t\tf_threadprivate \t= %d\n", f_threadprivate);
  }

  printf("\npo zakonczeniu 1 obszaru rownoleglego:\n");
  printf("\tf_threadprivate \t= %d\n", f_threadprivate);

#pragma omp parallel
#pragma omp critical
  {
    printf("\nw 2 obszarze równoległym: aktualna liczba watkow %d, moj ID %d\n",
           omp_get_num_threads(), omp_get_thread_num());
    printf("\n\tbez zmian:\n");
    printf("\t\tf_threadprivate \t= %d\n", f_threadprivate);
  }

  printf("\npo zakonczeniu 2 obszaru rownoleglego:\n");
  printf("\tf_threadprivate \t= %d\n", f_threadprivate);
}