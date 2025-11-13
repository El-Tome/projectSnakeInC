CC = gcc
CFLAGS  = -W -Wall -std=c89 -pedantic -O2 -Iinclude               # Flags imposé
CFLAGS += $(shell pkg-config --cflags MLV)                        # Flags MLV
LDFLAGS = $(shell pkg-config --libs-only-other --libs-only-L MLV) # Flags MLV
LDLIBS  = $(shell pkg-config --libs-only-l MLV)                   # Flags MLV

# Liste des fichiers sources et headers
# SRC = src/main.c src/controls.c 
# wildcard est une fonction qui permet de récupérer tous les fichiers dans le dossier  
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)
HEADERS = $(wildcard include/*.h) 

# Commande pour compiler l'exécutable
snake: $(OBJ)
	$(CC) -o snake $(OBJ) $(LDFLAGS) $(LDLIBS)

# Commande pour compiler les fichiers .c en .o
build/%.o: src/%.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@   # le $< est le fichier source (.c) et le $@ est le fichier objet (.o)

# Commande pour nettoyer les fichiers .o et l'exécutable
clean:
	rm -f build/*.o snake


