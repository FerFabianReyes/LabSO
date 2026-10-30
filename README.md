# Simulador de SO

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
- Cada cambio en su propia rama de Git 


## Colas y contexto de procesos

El archivo `colas.c` implementa las operaciones FIFO `crearCola`, `encolar`,
`extraer`, `colaVacia` y `liberarCola`. Cada PCB utiliza su campo `siguiente`
para enlazarse y debe pertenecer a una sola cola a la vez.

`cargarContexto(PCB *)` copia los registros guardados en el PCB hacia `reg`
(registros activos de CPU). `guardarContexto(PCB *)` realiza la copia inversa.
La memoria de registros reservada para cada PCB se libera en `liberarPCB`.

Para ejecutar la prueba independiente de colas y contexto:

    make test-colas

La prueba crea dos PCB de ejemplo, comprueba el orden FIFO, verifica la carga
y el guardado del contexto y comprueba que una cola vacía devuelva `NULL`.

## Carga de archivos y errores

`cargarArchivo(nombre, listos)` concentra el flujo de carga: crea el objeto
`Archivo`, lee el archivo, tokeniza sus renglones, valida la sintaxis, crea el
PCB y finalmente lo encola. Si falla cualquier etapa (incluidos errores
léxicos o sintácticos guardados en un renglón), libera lo reservado y no agrega
ningún PCB a la cola.

`mostrarErrorArchivo(cmd, nombre, error)` presenta el nombre del archivo y la
descripción del error en la ventana de Comandos.

Archivos de prueba incluidos:

- `prueba_bueno.asm`: instrucciones válidas y END.
- `prueba_vacio.asm`: archivo vacío.
- `prueba_lexico.asm`: literal decimal no permitido.
- `prueba_sintactico.asm`: instrucción con parámetros/formato incorrecto.
- `prueba_div_cero.asm`: sintaxis válida con división entre cero (error al ejecutar).
- `prueba_sin_end.asm`: programa válido que termina al llegar al fin del archivo.

Ejecuta `make test-carga` para probar la carga y comprobar que solo los tres
archivos válidos se agregan a la cola. El objetivo utiliza un encabezado mínimo
de ncurses en `test_support/` porque la prueba no necesita abrir ventanas.
