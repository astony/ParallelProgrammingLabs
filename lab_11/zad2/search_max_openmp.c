#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#include "search_max_openmp.h"

/********************** linear search ************************/
double search_max(double *A, long int p, long int k) {

  double a_max = A[p];
  long int i;
  for (i = p + 1; i <= k; i++)
    if (a_max < A[i])
      a_max = A[i];
  return (a_max);
}

/************* parallel linear search - openmp ****************/
double search_max_openmp_simple(double *A, long int p, long int k) {

  double a_max = A[p];
  double a_max_local = a_max;

#pragma omp parallel default(none) firstprivate(A, p, k,                       \
                                                a_max_local) shared(a_max)
  {
    long int i;
#pragma omp for
    for (i = p + 1; i <= k; i++)
      if (a_max_local < A[i])
        a_max_local = A[i];

#pragma omp critical(cs_a_max)
    {
      if (a_max < a_max_local)
        a_max = a_max_local;
    }
  }
  return (a_max);
}

/************* parallel linear search - openmp ****************/
double search_max_openmp_task(double *A, long int p, long int k) {

  double a_max = A[p];

#pragma omp parallel default(none) shared(a_max) firstprivate(A, p, k)
  {

#pragma omp single
    {
      long int num_threads = omp_get_num_threads();
      long int n = k - p + 1;

      long int num_tasks = num_threads + 1;
      long int n_loc = ceil(n / num_tasks);

      long int itask;
      for (itask = 0; itask < num_tasks; itask++) {
        long int p_task = p + itask * n_loc;
        if (p_task > k) {
          printf("Error in task decomposition! Exiting.\n");
          exit(0);
        }
        long int k_task = p + (itask + 1) * n_loc - 1;
        if (itask == num_tasks - 1)
          k_task = k;

#pragma omp task default(none) firstprivate(p_task, k_task) shared(A, a_max)
        {
          long int a_max_local = search_max(A, p_task, k_task);

#pragma omp critical
          {
            if (a_max < a_max_local)
              a_max = a_max_local;
          }
        } // end task definition
      }   // end loop over tasks
    }     // end single region
  }       // end parallel region

  return (a_max);
}

/************ binary search (array not sorted) ****************/
double bin_search_max(double *a, long int p, long int k) {

  if (p < k) {

    long int s = (p + k) / 2;

    double a_max_1 = bin_search_max(a, p, s);

    double a_max_2 = bin_search_max(a, s + 1, k);

    // printf("p %d  k %d, maximal elements %lf, %lf\n", p, k, a_max_1,
    // a_max_2);

    if (a_max_1 < a_max_2)
      return (a_max_2);
    else
      return (a_max_1);

  } else {
    return (a[p]);
  }
}

/*** single task for parallel binary search (array not sorted) - openmp ***/
#define max_level 6

double bin_search_max_task(double *a, long int p, long int k, long int level) {
  printf("thread %d, level %d\n", omp_get_thread_num(), level);
  level++;
  if (p < k) {

    long int s = (p + k) / 2;
    double a_max_1, a_max_2;

#pragma omp task final(level > max_level) default(none)                        \
                           firstprivate(p, s, level) shared(a, a_max_1)
    if (omp_in_final())
      a_max_1 = search_max(a, p, s);
    else
      a_max_1 = bin_search_max_task(a, p, s, level);

#pragma omp task final(level > max_level) default(none)                        \
                           firstprivate(s, k, level) shared(a, a_max_2)
    if (omp_in_final())
      a_max_2 = search_max(a, s + 1, k);
    else
      a_max_2 = bin_search_max_task(a, s + 1, k, level);

#pragma omp taskwait
    // printf("p %d  k %d, maximal elements %lf, %lf\n", p, k, a_max_1,
    // a_max_2);

    if (a_max_1 < a_max_2)
      return (a_max_2);
    else
      return (a_max_1);

  } else {
    return (a[p]);
  }
}

/********** parallel binary search (array not sorted) - openmp ***********/

double bin_search_max_openmp(double *A, long int p, long int k) {

  double a_max;

#pragma omp parallel default(none) firstprivate(A, p, k) shared(a_max)
  {
#pragma omp single
    {
      long int num_threads = omp_get_num_threads();
      long int n = k - p + 1;

      long int num_tasks = num_threads + 1;
      long int n_loc = ceil(n / num_tasks);

      long int itask;
      for (itask = 0; itask < num_tasks; itask++) {
        long int p_task = p + itask * n_loc;
        if (p_task > k) {
          printf("Error in task decomposition! Exiting.\n");
          exit(0);
        }
        long int k_task = p + (itask + 1) * n_loc - 1;
        if (itask == num_tasks - 1)
          k_task = k;

// printf("From %d to %d\n", p_task, k_task);

#pragma omp task default(none) shared(A, a_max) firstprivate(p_task, k_task)
        {
          long int a_max_local = bin_search_max_task(A, p_task, k_task, 0);

#pragma omp critical
          {
            if (a_max < a_max_local)
              a_max = a_max_local;
          }
        }
      }
    }
  }

  return (a_max);
}
