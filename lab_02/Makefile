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
fork: fork.o 
	$(LOADER) $(OPT) fork.o -o fork $(LIB)

# jak uzyskac plik fork.o ?
fork.o: fork.c libpomiar_czasu.a
	$(CCOMP) -c $(OPT) fork.c $(INC) 

# zaleznosci i komendy
clone: clone.o 
	$(LOADER) $(OPT) clone.o -o clone $(LIB)

# jak uzyskac plik clone.o ?
clone.o: clone.c libpomiar_czasu.a
	$(CCOMP) -c $(OPT) clone.c $(INC)

program: program.o
	$(LOADER) $(OPT) program.o -o program

program.o: program.c libpomiar_czasu.a
	$(CCOMP) -c $(OPT) program.c

libpomiar_czasu.a: ../pomiar_czasu/pomiar_czasu.o ../pomiar_czasu/pomiar_czasu.h
	$(AR) ../pomiar_czasu/libpomiar_czasu.a ../pomiar_czasu/pomiar_czasu.o

pomiar_czasu.o: ../pomiar_czasu/pomiar_czasu.c ../pomiar_czasu/pomiar_czasu.h
	$(CCOMP) -c $(OPT) ../pomiar_czasu/pomiar_czasu.c

clean:
	rm -f *.o && rm -f ../pomiar_czasu/*.o
