CC     = gcc
CFLAGS = -Wall -Wextra -std=gnu11
LIBS   = -lncursesw

# Todos los .c del proyecto, excepto las pruebas (test_*.c)
SRC = $(filter-out test_%.c, $(wildcard *.c))
OBJ = $(SRC:.c=.o)
EXE = proyecto

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $@

%.o: %.c prototipos.h estructuras.h
	$(CC) $(CFLAGS) -c $< -o $@

# ---- Pruebas (sin curses) ----
# Cada prueba nueva: agregar su regla y su nombre en TESTS.
TESTS = test_colas test_planificador

test: $(TESTS)
	@for t in $(TESTS); do ./$$t || exit 1; done

test_colas: test_colas.c colas.c estructuras.h
	$(CC) $(CFLAGS) test_colas.c colas.c -o $@

test_planificador: test_planificador.c planificador.c colas.c estructuras.h prototipos.h
	$(CC) $(CFLAGS) test_planificador.c planificador.c colas.c -o $@

clean:
	rm -f $(OBJ) $(EXE) $(TESTS)

.PHONY: all clean test
