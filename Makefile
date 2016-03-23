CC=gcc
CFLAGS=-W -Wall -ansi -pedantic -std=c99 -g
LDFLAGS=
EXEC=dpll
i=thing

all: $(EXEC)

$(EXEC): $(EXEC).o
	$(CC) -o $(EXEC) $(EXEC).o $(LDFLAGS)

$(EXEC).o: $(EXEC).c
	$(CC) -o $(EXEC).o -c $(EXEC).c $(CFLAGS)

clean:
	rm -rf *.o

cleanall: clean
	rm -rf $(EXEC)
	
help:
	@echo "make all : compile le projet"
	@echo "make clean : efface les fichiers objet"
	@echo "make cleanall : efface les fichiers objet et l'executable"
	@echo "make help : affiche ce message d'aide"
