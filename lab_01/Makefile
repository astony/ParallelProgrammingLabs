# kompilator c
CCOMP = gcc

# konsolidator
LOADER = gcc

# archiwizator bibliotek
AR = ar -rs

# opcje optymalizacji:
# wersja do debugowania
OPT = -g -DDEBUG
# wersja zoptymalizowana do mierzenia czasu
# OPT = -O3

# pliki naglowkowe
INC = -I../pomiar_czasu

# biblioteki
LIB = -L../pomiar_czasu -lpomiar_czasu -lm

# zaleznosci i komendy
moj_program: moj_program.o ../pomiar_czasu/pomiar_czasu.h
	$(LOADER) $(OPT) moj_program.o -o moj_program $(LIB) 

# jak uzyskac plik moj_program.o ?
moj_program.o: moj_program.c libpomiar_czasu.a
	$(CCOMP) -c $(OPT) moj_program.c $(INC)

libpomiar_czasu.a: ../pomiar_czasu/pomiar_czasu.o ../pomiar_czasu/pomiar_czasu.h
	$(AR) ../pomiar_czasu/libpomiar_czasu.a ../pomiar_czasu/pomiar_czasu.o

pomiar_czasu.o: ../pomiar_czasu/pomiar_czasu.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) ../pomiar_czasu/pomiar_czasu.c

clean:
	rm -f *.o && rm -f ../pomiar_czasu/*.o
