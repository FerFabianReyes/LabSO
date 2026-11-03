#include <locale.h>
#include "prototipos.h"

Registros *reg = NULL;

int main()
{
    int maxY, maxX;
    char cad[TAM_COMANDO];
    int pos = 0;
    int leyendo = 0;
    memset(cad, 0, sizeof(cad));

    setlocale(LC_ALL, "");      /* Para que se vean bien los acentos */
    initscr(); cbreak(); noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    getmaxyx(stdscr, maxY, maxX);

    WINDOW *ventanaEjecucion = crearVentana(maxY*8/27, maxX, 0, " Ejecución ");
    WINDOW *ventanaListos = crearVentana(maxY*9/27, maxX, maxY*8/27, " Listos ");
    WINDOW *ventanaTerminados = crearVentana(maxY*9/27, maxX, maxY*17/28, " Terminados ");
    WINDOW *ventanaComandos = crearVentana(maxY/10, maxX, maxY*11/12, " Comandos ");
    nodelay(ventanaComandos, TRUE);
    keypad(ventanaComandos, TRUE);
    limpiarComando(ventanaComandos);
    impEncabezado(ventanaEjecucion);
    impEncabezado(ventanaListos);
    impEncabezado(ventanaTerminados);

    reg = crearRegistro();
    Cola *listos = crearCola();
    Cola *terminados = crearCola();
    PCB *ejecucion = NULL;

    while (1)
    {
        /*---------- Planificador: si la CPU esta libre, entra el primero de Listos ----------*/
        if (ejecucion == NULL && !colaVacia(listos)) {
            ejecucion = extraer(listos);
            cargarContexto(ejecucion);
            ejecucion->estado = EJECUCION;
            ejecucion->espera = 0;
            limpiarFilas(ventanaEjecucion);
            impListos(ventanaListos, listos);
        }

        if (ejecucion != NULL) {
            if (ejecucion->espera < 125) {
                ejecucion->espera++;
            } else {
                ejecucion->espera = 0;
                int res = ejecutarPrograma(ejecucion);
                impEjecucion(ventanaEjecucion, ejecucion, res);

                /* Redibujar Ejecución puede mover el cursor físico de curses.
                   Si se está escribiendo un comando, devolverlo a Comandos. */
                if (leyendo) {
                    int altoComandos, anchoComandos;
                    getmaxyx(ventanaComandos, altoComandos, anchoComandos);
                    (void)altoComandos;
                    if (anchoComandos > 2) {
                        int columnaCursor = pos + 4;
                        if (columnaCursor >= anchoComandos - 1) {
                            columnaCursor = anchoComandos - 2;
                        }
                        wmove(ventanaComandos, 1, columnaCursor);
                        wrefresh(ventanaComandos);
                    }
                }

                if (ejecucion->estado == TERMINADO || ejecucion->IR->sig == NULL) {
                    guardarContexto(ejecucion);
                    ejecucion->estado = TERMINADO;
                    encolar(terminados, ejecucion);
                    ejecucion = NULL;
                    impTerminados(ventanaTerminados, terminados);
                } else {
                    ejecucion->IR = ejecucion->IR->sig;
                }
            }
        }

        /*---------- Lectura de comandos ----------*/
        if (kbhit()) {
            int caracter = wgetch(ventanaComandos);
            if (caracter == ERR) { napms(16); continue; }

            if (!leyendo) {
                leyendo = 1;
                pos = 0;
                memset(cad, 0, sizeof(cad));
                impVentanaComandos(ventanaComandos);
                curs_set(1);
            }

            if (caracter == '\n' || caracter == '\r' || caracter == KEY_ENTER) {
                cad[pos] = '\0';
                leyendo = 0;
                curs_set(0);

                limpiarComando(ventanaComandos);
                int comando = detectarComando(cad);

                if (comando == SALIR) { break; }

                if (comando == EJECUTAR_ARCHIVO) {
                    char *nomArchivo = sacarNomArchivo(cad);
                    int res = cargarArchivo(nomArchivo, listos);

                    if (res == BIEN) {
                        impListos(ventanaListos, listos);
                    } else {
                        mostrarErrorArchivo(ventanaComandos, nomArchivo, res);
                    }
                } else {
                    /* TODO: mostrarErrorComando(ventanaComandos, comando); */
                }
            } else {
                leerComando(ventanaComandos, &pos, cad, caracter);
            }
        }

        napms(16);
    }

    delwin(ventanaEjecucion);
    delwin(ventanaListos);
    delwin(ventanaTerminados);
    delwin(ventanaComandos);
    liberarCola(listos);
    liberarCola(terminados);
    liberarPCB(ejecucion);
    free(reg);
    endwin();
    return 0;
}