# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# opcje optymalizacji:
# wersja do debugowania
#OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
OPT = -O3 -fopenmp

# zaleznosci i komendy
calka: calka.o 
	$(LOADER) $(OPT) calka.o -o calka

# jak uzyskac plik pthreads_suma.o 
calka.o: calka.c
	$(CCOMP) -c $(OPT) calka.c

clean:
	rm -f *.o calka
