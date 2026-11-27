
CC     = gcc
CFLAGS = -Wall -Wextra -std=gnu11
LIBS   = -lncursesw

# Archivos del proyecto, excepto las pruebas
SRC = $(filter-out test_%.c, $(wildcard *.c))
OBJ = $(SRC:.c=.o)
EXE = proyecto

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $@

%.o: %.c prototipos.h estructuras.h
	$(CC) $(CFLAGS) -c $< -o $@

# ---- Pruebas (sin curses) ----

TESTS = test_colas test_planificador test_recursos

test: $(TESTS)
	@for t in $(TESTS); do ./$$t || exit 1; done

test_colas: test_colas.c colas.c estructuras.h
	$(CC) $(CFLAGS) test_colas.c colas.c -o $@

test_planificador: test_planificador.c planificador.c colas.c estructuras.h prototipos.h
	$(CC) $(CFLAGS) test_planificador.c planificador.c colas.c -o $@

test_recursos: test_recursos.c recursos.c estructuras.h prototipos.h
	$(CC) $(CFLAGS) test_recursos.c recursos.c -o $@

clean:
	rm -f $(OBJ) $(EXE) $(TESTS)

.PHONY: all clean test
