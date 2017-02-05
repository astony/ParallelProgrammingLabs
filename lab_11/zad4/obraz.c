#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct Obraz {
  int size_n;
  int size_m;
  char **tab;
  int *histogram;
};

void clear_histogram(struct Obraz *obraz) {
  int i;
  for (i = 0; i < 94; i++)
    obraz->histogram[i] = 0;
}

void inicjalizuj_obraz(struct Obraz *obraz, int n, int m) {
  obraz->size_n = n;
  obraz->size_m = m;
  obraz->tab = malloc(n * sizeof(char *));
  int i, j;
  for (i = 0; i < n; ++i)
    obraz->tab[i] = malloc(m * sizeof(char));

  for (i = 0; i < n; i++) {
    for (j = 0; j < m; j++) {
      obraz->tab[i][j] = (char)rand() % 128 + 33; // ascii 33-127
      // printf("%c ", obraz->tab[i][j]);
    }
    // printf("\n");
  }

  // printf("\n\n");

  obraz->histogram = (int *)malloc(94 * sizeof(int));
  clear_histogram(obraz);
}

void calculate_histogram(struct Obraz *obraz) {
  int i, j, k;

#pragma omp parallel for ordered num_threads(4)                                \
    schedule(dynamic) private(i, j, k) collapse(3)
  for (i = 0; i < obraz->size_n; i++)
    for (j = 0; j < obraz->size_m; j++)
      for (k = 0; k < 94; k++) {
        if (obraz->tab[i][j] == (char)(k + 33))
          obraz->histogram[k]++;
        printf("id: %d\n", omp_get_thread_num());
      }
}

void print_histogram(struct Obraz *obraz) {
  int i;
  printf("\n");
  for (i = 0; i < 94; i++)
    printf("%c %d\n", (char)(i + 33), obraz->histogram[i]);
}

int main() {
  struct Obraz obraz;
  inicjalizuj_obraz(&obraz, 100, 100);
  calculate_histogram(&obraz);
  print_histogram(&obraz);
  return 0;
}
