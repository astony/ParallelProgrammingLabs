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
obraz: obraz.o 
	$(LOADER) $(OPT) obraz.o -o obraz $(LIB)

# jak uzyskac plik openmp_for.o ?
obraz.o: obraz.c
	$(CCOMP) -c $(OPT) obraz.c $(INC) 

clean:
	rm -f *.o obraz
