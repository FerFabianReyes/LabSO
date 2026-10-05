#include <locale.h>
#include "prototipos.h"

Registros *reg = NULL;

int main()
{
    int maxY, maxX;
    char cad[50];
    int pos = 0;
    int leyendo = 0;
    memset(cad, 0, sizeof(cad));

    setlocale(LC_ALL, "");      /* Para que se vean bien los acentos */
    initscr(); cbreak(); noecho();
    keypad(stdscr, TRUE);
    curs_set(0);
    getmaxyx(stdscr, maxY, maxX);

    WINDOW *ventanaDatos = crearVentana(maxY*3/5, maxX, 0, " Datos ");
    WINDOW *ventanaErrores = crearVentana(maxY/5, maxX, maxY*3/5, " Errores ");
    WINDOW *ventanaComandos = crearVentana(maxY/5, maxX, maxY*4/5, " Comandos ");
    nodelay(ventanaComandos, TRUE);
    keypad(ventanaComandos, TRUE);
    limpiarComando(ventanaComandos);
    impEncabezado(ventanaDatos);

    reg = crearRegistro();
    Archivo *archivo = crearArchivo();
    Ejecucion *ejecucion = crearEjecucion(archivo, "");

    while (1)
    {
        /*---------- Ejecucion de una instruccion ----------*/
        if (ejecucion->estado == EJECUCION)
        {
            if (!ejecucion->IR) {
                /* Fin del archivo: el programa termina son necesidad de END */
                ejecucion->estado = ESPERA;
            } else if (ejecucion->espera < 125) {
                ejecucion->espera++;
            } else {
                ejecucion->espera = 0;
                int res = ejecutarPrograma(ejecucion);
                impInstruccVentana(ventanaDatos, ejecucion, res);
                if (res != BIEN) { detectarError(ventanaErrores, res); }
                ejecucion->IR = ejecucion->IR->sig;
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

                limpiarVentana(ventanaErrores, " Errores ");
                limpiarComando(ventanaComandos);
                int comando = detectarComando(cad);

                if (comando == SALIR) { break; }

                if (comando == EJECUTAR_ARCHIVO) {
                    char *nomArchivo = sacarNomArchivo(cad);

                    liberarEjecucion(ejecucion);
                    archivo = crearArchivo();
                    ejecucion = crearEjecucion(archivo, nomArchivo);
                    limpiarVentana(ventanaDatos, " Datos ");
                    impEncabezado(ventanaDatos);

                    /* Solo se detiene si el archivo no existe o esta vacio.
                       Los errores de casa renglon se muestran al ejecutarlo. */
                    int res = leerArchivo(nomArchivo, archivo);
                    if (res == BIEN) { res = tokenizar(archivo); }
                    if (res == BIEN) { res = verifSintaxis(archivo); }

                    if (res != BIEN) {
                        detectarError(ventanaErrores, res);
                    } else {
                        ejecucion->IR = archivo->inicio;
                        ejecucion->PC = 0;
                        ejecucion->espera = 0;
                        ejecucion->estado = EJECUCION;
                    }
                } else {
                    detectarError(ventanaErrores, comando);
                }
            } else {
                leerComando(ventanaComandos, &pos, cad, caracter);
            }
        }

        napms(16);
    }

    delwin(ventanaDatos);
    delwin(ventanaErrores);
    delwin(ventanaComandos);
    liberarEjecucion(ejecucion);
    free(reg);
    endwin();
    return 0;
}
