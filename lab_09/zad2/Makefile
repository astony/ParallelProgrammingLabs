# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG  -fopenmp
# wersja zoptymalizowana do mierzenia czasu
# OPT = -O3

# pliki naglowkowe
INC =

# biblioteki
LIB = -lm

# zaleznosci i komendy
openmp_for: openmp_for.o 
	$(LOADER) $(OPT) openmp_for.o -o openmp_for $(LIB)

# jak uzyskac plik openmp_for.o ?
openmp_for.o: openmp_for.c
	$(CCOMP) -c $(OPT) openmp_for.c $(INC) 

clean:
	rm -f *.o
