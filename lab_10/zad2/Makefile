# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# archiwizator bibliotek
AR = ar -rs

# opcje optymalizacji:
# wersja do debugowania
# OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
OPT = -O3 -fopenmp

# pliki naglowkowe
INC = -I../../pomiar_czasu

# biblioteki
LIB = -L../../pomiar_czasu -lpomiar_czasu -lm

openmp_mat_vec: openmp_mat_vec.o mat_vec.o pomiar_czasu.o
	$(LOADER) $(OPT) openmp_mat_vec.o mat_vec.o  pomiar_czasu.o -o openmp_mat_vec $(LIB)

openmp_mat_vec.o: openmp_mat_vec.c libpomiar_czasu.a
	$(CCOMP) -c $(OPT) openmp_mat_vec.c

mat_vec.o: mat_vec.c
	$(CCOMP) -c  $(OPT) mat_vec.c

libpomiar_czasu.a: ../../pomiar_czasu/pomiar_czasu.o ../../pomiar_czasu/pomiar_czasu.h
	$(AR) ../../pomiar_czasu/libpomiar_czasu.a ../../pomiar_czasu/pomiar_czasu.o

pomiar_czasu.o: ../../pomiar_czasu/pomiar_czasu.c ../../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) ../../pomiar_czasu/pomiar_czasu.c

clean:
	rm -f *.o ../../pomiar_czasu/libpomiar_czasu.a ../../pomiar_czasu/*.o openmp_mat_vec
