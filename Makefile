CC     = gcc
CFLAGS = -Wall -Wextra -std=gnu11
LIBS   = -lncursesw

SRC = main.c archivos.c colas.c comandos.c instrucciones.c lexicoSintactico.c \
      logErrores.c tokens.c ventanas.c
OBJ = $(SRC:.c=.o)
EXE = proyecto

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $@

%.o: %.c prototipos.h estructuras.h
	$(CC) $(CFLAGS) -c $< -o $@

# Prueba aislada de las colas y el intercambio de contexto.
test-colas: colas.c test_colas.c estructuras.h prototipos.h
	$(CC) $(CFLAGS) -DPRUEBA_COLAS colas.c test_colas.c -o prueba_colas
	./prueba_colas

clean:
	rm -f $(OBJ) $(EXE) prueba_colas

.PHONY: all clean test-colas
