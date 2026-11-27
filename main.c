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
    int enPantalla = -1;        /* id del proceso que muestra Ejecucion */

    impNuevos(pantalla.nuevos, nuevos);
    impBloqueados(pantalla.bloqueados, bloqueados);
    impRecursos(pantalla.ejecucion);

    while (1)
    {
        /*---------- Planificador: si la CPU esta libre, entra el primero de Listos ----------*/
        if (ejecucion == NULL && !colaVacia(listos)) {
            ejecucion = extraer(listos);
            cargarContexto(ejecucion);
            ejecucion->estado = EJECUCION;
            ejecucion->espera = 0;
            ejecucion->ejecutadas = 0;              /* quantum nuevo */
            /* Si es el mismo proceso que ya se ve en pantalla (volvio de
               Listos tras su quantum) no se borra la fila: evita parpadeo. */
            if (ejecucion->id != enPantalla) { limpiarFilas(pantalla.ejecucion); }
            impListos(pantalla.listos, listos);
        }

        if (ejecucion != NULL) {
            if (ejecucion->espera < 125) {
                ejecucion->espera++;
            } else {
                ejecucion->espera = 0;
                int res = ejecutarPrograma(ejecucion);
                impEjecucion(pantalla.ejecucion, ejecucion, res);
                enPantalla = ejecucion->id;

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

                /* Error en la instruccion, END o ultimo renglon: Terminados.
                   Se guardan los registros calculados hasta este momento. */
                if (res != BIEN || ejecucion->estado == TERMINADO || ejecucion->IR->sig == NULL) {
                    guardarContexto(ejecucion);
                    ejecucion->estado = TERMINADO;
                    ejecucion->estatusFinal = res;      /* BIEN o el error */
                    encolar(terminados, ejecucion);
                    ejecucion = NULL;
                    impTerminados(pantalla.terminados, terminados);
                } else {
                    ejecucion->IR = ejecucion->IR->sig;
                    /* Quantum: Q instrucciones por turno, luego cede la CPU. */
                    ejecucion->ejecutadas++;
                    if (ejecucion->ejecutadas >= Q) {
                        guardarContexto(ejecucion);
                        ejecucion->estado = ESPERA;
                        ejecucion->ejecutadas = 0;
                        encolar(listos, ejecucion);
                        ejecucion = NULL;
                        impListos(pantalla.listos, listos);
                    }
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

                if (comando == KILL_PROCESO) {
                    int id = idKill(cad);
                    if (id < 0) {
                        mostrarErrorComando(pantalla.comandos, COMANDO_INVALIDO);
                    } else if (matarProceso(id, &ejecucion, listos, terminados) == BIEN) {
                        if (ejecucion == NULL) {
                            limpiarFilas(pantalla.ejecucion);
                            wrefresh(pantalla.ejecucion);
                            enPantalla = -1;
                        }
                        impListos(pantalla.listos, listos);
                        impTerminados(pantalla.terminados, terminados);
                    } else {
                        mostrarErrorKill(pantalla.comandos, id);
                    }
                } else if (comando == EJECUTAR_ARCHIVO) {
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
                    impBloqueados(pantalla.bloqueados, bloqueados);
                    impRecursos(pantalla.ejecucion);
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