# Simulador de SO (proyecto unificado E1 + E2)

## Requisitos
Linux (o WSL en Windows) con ncurses:

    sudo apt install build-essential libncurses-dev

## Compilar y ejecutar

    make
    ./proyecto

Comandos dentro del programa:

    ejecutar a.asm
    salir

`make clean` borra los objetos y el ejecutable.

## Estructura

| Archivo              | Contenido                                                  |
|----------------------|------------------------------------------------------------|
| estructuras.h        | Estructuras (Token, Renglon, Archivo, Ejecucion) y enums   |
| prototipos.h         | Prototipos de todas las funciones (solo prototipos)        |
| main.c               | Ciclo principal: ejecucion y lectura de comandos           |
| archivos.c           | Lectura del archivo .asm a una lista de renglones          |
| tokens.c             | Separacion de cada renglon en tokens                       |
| lexicoSintactico.c   | Validacion lexica y sintactica de cada renglon             |
| instrucciones.c      | Ejecucion de MOV, ADD, SUB, MUL, DIV, INC, DEC             |
| ventanas.c           | Dibujo de las ventanas y de la tabla de datos (ncurses)    |
| comandos.c           | Lectura e interpretacion de comandos                       |
| logErrores.c         | Mensajes de error y textos de la columna Status            |

## Reglas del lenguaje
- Instrucciones: MOV, ADD, SUB, MUL, DIV (dos parametros) e INC, DEC (uno).
- Registros: AX, BX, CX, DX, en mayusculas.
- El programa termina al llegar al final del archivo (no se usa END).
- Un renglon con error no detiene el programa: se muestra en Status y se
  continua con el siguiente.
- DIV es division entera que trunca hacia cero (-7 / 2 = -3).

## Reglas del equipo
- Las funciones van en archivos .c; en los .h solo van prototipos,
  estructuras y constantes.
- Cada cambio en su propia rama de Git y revisado por alguien mas antes
  de integrarlo.
