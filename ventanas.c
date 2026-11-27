#include "prototipos.h"

/*---------- Columnas de la tabla de datos ---------- */
#define NUM_COLUMNAS 9
static const char *encabezado[NUM_COLUMNAS] = {"ID", "Nombre", "PC", "AX", "BX", "CX", "DX", "IR", "Status"};
static const int pesos[NUM_COLUMNAS]        = { 3,    5,        3,    3,    3,    3,    3,    7,    9 };
enum { COL_ID, COL_NOMBRE, COL_PC, COL_AX, COL_BX, COL_CX, COL_DX, COL_IR, COL_STATUS };


static int bordeColumna(int i, int anchoTabla)
{
    int total = 0, acumulado = 0;
    for (int k = 0; k < NUM_COLUMNAS; k++) {
        total += pesos[k];
        if (k < i) { acumulado += pesos[k]; }
    }
    return (anchoTabla - 1) * acumulado / total;
}


static void imprimirCelda(WINDOW *ventana, int fila, int desp, int anchoTabla,
                          int col, const char *texto, int centrar)
{
    int inicio = bordeColumna(col, anchoTabla) + 1;
    int ancho = bordeColumna(col + 1, anchoTabla) - inicio;
    if (ancho <= 0) { return; }
    if (!texto) { texto = ""; }

    /* Limpia la celda antes de escribir */
    mvwprintw(ventana, fila, desp + inicio, "%*s", ancho, "");

    int x = inicio + 1;                     /* un espacio de margen */
    int len = (int)strlen(texto);
    if (centrar && len < ancho) { x = inicio + (ancho - len) / 2; }

    int disponible = inicio + ancho - x;
    if (disponible > 0) {
        mvwprintw(ventana, fila, desp + x, "%.*s", disponible, texto);
    }
}

static void imprimirNumero(WINDOW *ventana, int fila, int desp, int anchoTabla, int col, int valor)
{
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "%d", valor);
    imprimirCelda(ventana, fila, desp, anchoTabla, col, buffer, 0);
}

int kbhit(void)
{
    struct timeval tv;
    fd_set read_fd;
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    FD_ZERO(&read_fd);
    FD_SET(0, &read_fd);

    if (select(1, &read_fd, NULL, NULL, &tv) == -1)
        return 0;

    if (FD_ISSET(0, &read_fd))
        return 1;

    return 0;
}

WINDOW *crearVentana(int altura, int anchura, int posY, char *nombre)
{
    WINDOW *ventana = newwin(altura, anchura, posY, 0);
    box(ventana, 0, 0);
    mvwprintw(ventana, 0, 2, "%s", nombre);
    wrefresh(ventana);
    return ventana;
}

void impVentanaComandos(WINDOW *ventana)
{
    mvwprintw(ventana, 1, 1, " > ");
    wclrtoeol(ventana);
    box(ventana, 0, 0);
    mvwprintw(ventana, 0, 2, " Comandos ");
    wrefresh(ventana);
}

void limpiarComando(WINDOW *ventana)
{
    wmove(ventana, 1, 1);
    wclrtoeol(ventana);
    box(ventana, 0, 0);
    mvwprintw(ventana, 0, 2, " Comandos ");
    mvwprintw(ventana, 1, 1, " > ");
    wrefresh(ventana);
}

void impEncabezado(WINDOW *ventana)
{
    int maxY, maxX;
    getmaxyx(ventana, maxY, maxX);
    (void)maxY;
    int anchoTabla = maxX - 2;

    WINDOW *subEnc = derwin(ventana, 3, anchoTabla, 1, 1);
    if (!subEnc) { return; }
    box(subEnc, 0, 0);

    for (int col = 1; col < NUM_COLUMNAS; col++) {
        int x = bordeColumna(col, anchoTabla);
        mvwaddch(subEnc, 0, x, ACS_TTEE);
        mvwaddch(subEnc, 1, x, ACS_VLINE);
        mvwaddch(subEnc, 2, x, ACS_BTEE);
    }

    for (int col = 0; col < NUM_COLUMNAS; col++) {
        imprimirCelda(subEnc, 1, 0, anchoTabla, col, encabezado[col], 1);
    }

    wrefresh(subEnc);
    delwin(subEnc);
    touchwin(ventana);
    wrefresh(ventana);
}

/* Muestra únicamente el estado de la instrucción que se está ejecutando.
   La fila es fija y se limpia el área de datos para no conservar historial. */
void impEjecucion(WINDOW *ventana, PCB *proceso, int resultado)
{
    int maxY, maxX;
    getmaxyx(ventana, maxY, maxX);
    if (maxY <= FILA_INICIAL || maxX <= 2 || !proceso) { return; }

    int anchoTabla = maxX - 2;
    const char *instruccion = (proceso->IR && proceso->IR->texto)
                                ? proceso->IR->texto : "";

    /* Borra las instrucciones anteriores: la ventana conserva solo una fila. */
    limpiarFilas(ventana);

    imprimirNumero(ventana, FILA_INICIAL, 1, anchoTabla, COL_ID, proceso->id);
    imprimirCelda (ventana, FILA_INICIAL, 1, anchoTabla, COL_NOMBRE, proceso->nombre, 0);
    imprimirNumero(ventana, FILA_INICIAL, 1, anchoTabla, COL_PC, proceso->PC);
    imprimirNumero(ventana, FILA_INICIAL, 1, anchoTabla, COL_AX, reg->EAX);
    imprimirNumero(ventana, FILA_INICIAL, 1, anchoTabla, COL_BX, reg->EBX);
    imprimirNumero(ventana, FILA_INICIAL, 1, anchoTabla, COL_CX, reg->ECX);
    imprimirNumero(ventana, FILA_INICIAL, 1, anchoTabla, COL_DX, reg->EDX);
    imprimirCelda (ventana, FILA_INICIAL, 1, anchoTabla, COL_IR, instruccion, 0);
    imprimirCelda (ventana, FILA_INICIAL, 1, anchoTabla, COL_STATUS,
                   mensajeEstatus(resultado), 0);

    wrefresh(ventana);
}

void limpiarFilas(WINDOW *ventana) {
    int maxY, maxX;
    getmaxyx(ventana, maxY, maxX);
    if (maxX <= 2 || maxY <= FILA_INICIAL) { return; }
    for (int fila = FILA_INICIAL; fila < maxY - 1; fila++) {
        mvwhline(ventana, fila, 1, ' ', maxX - 2);
    }
}

void imprimirFilaPCB(WINDOW *ventana, int fila, int anchoTabla,
                            PCB *p, const char *ir, const char *status) {
    int desp = 1;
    Registros *r = p->registro;

    imprimirNumero(ventana, fila, desp, anchoTabla, COL_ID,     p->id);
    imprimirCelda (ventana, fila, desp, anchoTabla, COL_NOMBRE, p->nombre, 0);
    imprimirNumero(ventana, fila, desp, anchoTabla, COL_PC,     p->PC);
    imprimirNumero(ventana, fila, desp, anchoTabla, COL_AX,     r ? r->EAX : 0);
    imprimirNumero(ventana, fila, desp, anchoTabla, COL_BX,     r ? r->EBX : 0);
    imprimirNumero(ventana, fila, desp, anchoTabla, COL_CX,     r ? r->ECX : 0);
    imprimirNumero(ventana, fila, desp, anchoTabla, COL_DX,     r ? r->EDX : 0);
    imprimirCelda (ventana, fila, desp, anchoTabla, COL_IR,     ir, 0);
    imprimirCelda (ventana, fila, desp, anchoTabla, COL_STATUS, status, 0);
}

/* Modos de imprimirCola (el tercer parametro conserva su firma). */
enum { MODO_COLA = 0, MODO_TERMINADOS = 1, MODO_BLOQUEADOS = 2 };

void imprimirCola(WINDOW *ventana, Cola *cola, int modo) {
    if (!ventana) { return; }

    int maxY, maxX;
    getmaxyx(ventana, maxY, maxX);
    int anchoTabla = maxX - 2;
    int disponibles = maxY - 1 - FILA_INICIAL;

    limpiarFilas(ventana);

    if (cola && disponibles > 0) {
        int fila = FILA_INICIAL;

        for (PCB *p = cola->frente; p; p = p->siguiente) {
            if (fila >= maxY - 1) { fila = FILA_INICIAL; }

            const char *ir = "-";
            const char *status = "-";
            if (modo == MODO_TERMINADOS) {
                ir = p->IR ? p->IR->texto : "-";
                /* BIEN -> "Terminado"; si no, el error o "Killed" */
                status = (p->estatusFinal == BIEN)
                             ? "Terminado" : mensajeEstatus(p->estatusFinal);
            } else if (modo == MODO_BLOQUEADOS) {
                ir = p->IR ? p->IR->texto : "-";    /* el GET que lo bloqueo */
                status = "Bloqueado";
            }
            imprimirFilaPCB(ventana, fila, anchoTabla, p, ir, status);
            fila++;
        }
    }

    wrefresh(ventana);
}

void impListos(WINDOW *ventana, Cola *cola)
{
    imprimirCola(ventana, cola, 0);
}

void impTerminados(WINDOW *ventana, Cola *cola)
{
    imprimirCola(ventana, cola, 1);
}
/*
 * Tamano minimo de terminal: 33 filas x 80 columnas.
 *   Ejecucion 6 + 4 tablas de 6 filas (cada una con 1 fila de datos) + Comandos 3.
 * Recomendado: 40 filas o mas para ver varios procesos por tabla.
 * Si la terminal es mas chica, crearPantalla avisa y termina el programa.
 */
#define MIN_FILAS     33
#define MIN_COLUMNAS  80
#define ALTO_COMANDOS 3
#define ALTO_EJECUCION 6

/*
 * crearPantalla: el diseno de la pantalla vive aqui, no en main.c.
 * Orden (arriba a abajo): Ejecucion, Nuevos, Listos, Bloqueados,
 * Terminados y Comandos. Las 4 tablas se reparten el espacio restante.
 */
Pantalla crearPantalla(void)
{
    int maxY, maxX;
    getmaxyx(stdscr, maxY, maxX);

    if (maxY < MIN_FILAS || maxX < MIN_COLUMNAS) {
        endwin();
        fprintf(stderr,
                "Terminal muy chica (%d filas x %d columnas).\n"
                "Se necesitan al menos %d filas x %d columnas.\n",
                maxY, maxX, MIN_FILAS, MIN_COLUMNAS);
        exit(1);
    }

    int resto = maxY - ALTO_COMANDOS - ALTO_EJECUCION;
    int base = resto / 4;
    int sobran = resto % 4;
    int altoNuevos     = base + (sobran > 0);
    int altoListos     = base + (sobran > 1);
    int altoBloqueados = base + (sobran > 2);
    int altoTerminados = base;

    int y = 0;
    Pantalla p;
    p.ejecucion  = crearVentana(ALTO_EJECUCION, maxX, y, " Ejecución ");   y += ALTO_EJECUCION;
    p.nuevos     = crearVentana(altoNuevos, maxX, y, " Nuevos ");          y += altoNuevos;
    p.listos     = crearVentana(altoListos, maxX, y, " Listos ");          y += altoListos;
    p.bloqueados = crearVentana(altoBloqueados, maxX, y, " Bloqueados ");  y += altoBloqueados;
    p.terminados = crearVentana(altoTerminados, maxX, y, " Terminados ");  y += altoTerminados;
    p.comandos   = crearVentana(ALTO_COMANDOS, maxX, y, " Comandos ");
    keypad(p.comandos, TRUE);

    limpiarComando(p.comandos);
    impEncabezado(p.ejecucion);
    impEncabezado(p.nuevos);
    impEncabezado(p.listos);
    impEncabezado(p.bloqueados);
    impEncabezado(p.terminados);
    impRecursos(p.ejecucion);
    return p;
}

void impNuevos(WINDOW *ventana, Cola *cola)
{
    imprimirCola(ventana, cola, MODO_COLA);
}

void impBloqueados(WINDOW *ventana, Cola *cola)
{
    imprimirCola(ventana, cola, MODO_BLOQUEADOS);
}

void impRecursos(WINDOW *ventana)
{
    if (!ventana) { return; }

    int maxY, maxX;
    getmaxyx(ventana, maxY, maxX);
    (void)maxY;

    char texto[48];
    snprintf(texto, sizeof(texto), " Recursos: X=%d Y=%d Z=%d ",
             recursos[0], recursos[1], recursos[2]);
    int len = (int)strlen(texto);
    int x = maxX - len - 2;
    if (x <= 14) { return; }                 /* no tapar el titulo */

    mvwhline(ventana, 0, x, ACS_HLINE, len); /* borra el texto anterior */
    mvwprintw(ventana, 0, x, "%s", texto);
    wrefresh(ventana);
}