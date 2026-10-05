CC     = gcc
CFLAGS = -Wall -Wextra -std=gnu11
LIBS   = -lncursesw

SRC = main.c archivos.c comandos.c instrucciones.c lexicoSintactico.c \
      logErrores.c tokens.c ventanas.c
OBJ = $(SRC:.c=.o)
EXE = proyecto

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LIBS) -o $@

%.o: %.c prototipos.h estructuras.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXE)

.PHONY: all clean
