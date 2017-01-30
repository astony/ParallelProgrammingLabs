# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# archiwizator bibliotek
AR = ar -rs

# opcje optymalizacji:
# wersja do debugowania
#OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
OPT = -O3 -fopenmp

# pliki naglowkowe
INC = -I../pomiar_czasu

# biblioteki
#LIB = -L../pomiar_czasu -lpomiar_czasu -lm -lpthread
LIB = -L../pomiar_czasu -lpomiar_czasu -lm -lpthread

# zaleznosci i komendy
pthreads_suma: pthreads_suma.o 
	$(LOADER) $(OPT) pthreads_suma.o -o pthreads_suma $(LIB)

# jak uzyskac plik pthreads_suma.o 
pthreads_suma.o: pthreads_suma.c libpomiar_czasu.a
	$(CCOMP) -c $(OPT) pthreads_suma.c $(INC)

libpomiar_czasu.a: ../pomiar_czasu/pomiar_czasu.o ../pomiar_czasu/pomiar_czasu.h
	$(AR) ../pomiar_czasu/libpomiar_czasu.a ../pomiar_czasu/pomiar_czasu.o

pomiar_czasu.o: ../pomiar_czasu/pomiar_czasu.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) ../pomiar_czasu/pomiar_czasu.c

clean:
	rm -f *.o
