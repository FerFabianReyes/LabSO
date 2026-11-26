#include <locale.h>
#include "prototipos.h"

Registros *reg = NULL;
int Q = 4;                              /* quantum: instrucciones por turno */
int recursos[NUM_RECURSOS];             /* recursos disponibles del sistema */

int main()
{
    char cad[TAM_COMANDO];
    int pos = 0;
    int leyendo = 0;
    memset(cad, 0, sizeof(cad));

    setlocale(LC_ALL, "");      /* Para que se vean bien los acentos */
    initscr(); cbreak(); noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    Pantalla pantalla = crearPantalla();

    reg = crearRegistro();
    recursosIniciar();
    Cola *nuevos = crearCola();
    Cola *listos = crearCola();
    Cola *bloqueados = crearCola();
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
            limpiarFilas(pantalla.ejecucion);
            impListos(pantalla.listos, listos);
        }

        if (ejecucion != NULL) {
            if (ejecucion->espera < 125) {
                ejecucion->espera++;
            } else {
                ejecucion->espera = 0;
                int res = ejecutarPrograma(ejecucion);
                impEjecucion(pantalla.ejecucion, ejecucion, res);

                /* Redibujar Ejecución puede mover el cursor físico de curses.
                   Si se está escribiendo un comando, devolverlo a Comandos. */
                if (leyendo) {
                    int altoComandos, anchoComandos;
                    getmaxyx(pantalla.comandos, altoComandos, anchoComandos);
                    (void)altoComandos;
                    if (anchoComandos > 2) {
                        int columnaCursor = pos + 4;
                        if (columnaCursor >= anchoComandos - 1) {
                            columnaCursor = anchoComandos - 2;
                        }
                        wmove(pantalla.comandos, 1, columnaCursor);
                        wrefresh(pantalla.comandos);
                    }
                }

                if (ejecucion->estado == TERMINADO || ejecucion->IR->sig == NULL) {
                    guardarContexto(ejecucion);
                    ejecucion->estado = TERMINADO;
                    encolar(terminados, ejecucion);
                    ejecucion = NULL;
                    impTerminados(pantalla.terminados, terminados);
                } else {
                    ejecucion->IR = ejecucion->IR->sig;
                }
            }
        }

        /*---------- Lectura de comandos ----------*/
        if (kbhit()) {
            int caracter = wgetch(pantalla.comandos);
            if (caracter == ERR) { napms(16); continue; }

            if (!leyendo) {
                leyendo = 1;
                pos = 0;
                memset(cad, 0, sizeof(cad));
                impVentanaComandos(pantalla.comandos);
                curs_set(1);
            }

            if (caracter == '\n' || caracter == '\r' || caracter == KEY_ENTER) {
                cad[pos] = '\0';
                leyendo = 0;
                curs_set(0);

                limpiarComando(pantalla.comandos);
                int comando = detectarComando(cad);

                if (comando == SALIR) { break; }

                if (comando == EJECUTAR_ARCHIVO) {
                    char *nombres[MAX_ARCHIVOS];
                    int errores[MAX_ARCHIVOS];
                    int fallo = 0;
                    int n = separarArchivos(cad, nombres, MAX_ARCHIVOS);

                    for (int i = 0; i < n; i++) {
                        errores[i] = cargarArchivo(nombres[i], nuevos, terminados);
                        if (errores[i] != BIEN) { fallo = 1; }
                    }

                    largoPlazo(nuevos, listos);
                    impNuevos(pantalla.nuevos, nuevos);
                    impListos(pantalla.listos, listos);
                    if (fallo) { mostrarErroresCarga(pantalla.comandos, nombres, errores, n); }
                } else {
                    mostrarErrorComando(pantalla.comandos, comando);
                }
            } else {
                leerComando(pantalla.comandos, &pos, cad, caracter);
            }
        }

        napms(16);
    }

    delwin(pantalla.ejecucion);
    delwin(pantalla.listos);
    delwin(pantalla.terminados);
    delwin(pantalla.comandos);
    if (pantalla.nuevos) { delwin(pantalla.nuevos); }
    if (pantalla.bloqueados) { delwin(pantalla.bloqueados); }
    liberarCola(nuevos);
    liberarCola(bloqueados);
    liberarCola(listos);
    liberarCola(terminados);
    liberarPCB(ejecucion);
    free(reg);
    endwin();
    return 0;
}