#ifndef _search_max_openmp_h

#define _search_max_openmp_h

#ifdef __cplusplus
extern "C" {
#endif

double search_max(double *A, long int p, long int k);

double search_max_openmp_simple(double *A, long int p, long int k);

double search_max_openmp_task(double *A, long int p, long int k);

double bin_search_max(double *A, long int p, long int k);

double bin_search_max_openmp(double *A, long int p, long int k);

#ifdef __cplusplus
}
#endif

#endif
