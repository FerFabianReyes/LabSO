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
    Archivo *archivo = crearArchivo();
    PCB *PCB = crearPCB(archivo, "");

    while (1)
    {
        /*---------- Ejecucion de una instruccion ----------*/
        if (PCB->estado == EJECUCION)
        {
            if (PCB->espera < 125) {
                PCB->espera++;
            } else {
                PCB->espera = 0;
                int res = ejecutarPrograma(PCB);
                impEjecucion(ventanaEjecucion, PCB, res);

                if (PCB->estado == TERMINADO) { PCB->estado = ESPERA;}
                else if (PCB->IR->sig == NULL)
                {
                    PCB->estado = TERMINADO;
                    PCB->estado = ESPERA;
                } else { PCB->IR = PCB->IR->sig; }
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

                    liberarPCB(PCB);
                    archivo = crearArchivo();
                    PCB = crearPCB(archivo, nomArchivo);
                    limpiarVentana(ventanaEjecucion, " Datos ");
                    impEncabezado(ventanaEjecucion);

                    /* Solo se detiene si el archivo no existe o esta vacio.
                       Los errores de casa renglon se muestran al ejecutarlo. */
                    int res = leerArchivo(nomArchivo, archivo);
                    if (res == BIEN) { res = tokenizar(archivo); }
                    if (res == BIEN) { res = verifSintaxis(archivo); }

                    if (res == BIEN) {
                        PCB->IR = archivo->inicio;
                        PCB->PC = 0;
                        PCB->espera = 0;
                        PCB->estado = EJECUCION;
                    } 
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
    liberarPCB(PCB);
    free(reg);
    endwin();
    return 0;
}