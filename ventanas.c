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

/* Borra todo el contenido de la ventana y vuelve a dibujar el marco */
void limpiarVentana(WINDOW *ventana, char *nomVentana)
{
    werase(ventana);
    box(ventana, 0, 0);
    mvwprintw(ventana, 0, 2, "%s", nomVentana);
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

/* Agrega una fila a la tabla con el estado despues de ejecutar el renglon */
void impInstruccVentana(WINDOW *ventana, Ejecucion *ejecucion, int resultado)
{
    static int fila = 4;
    static Ejecucion *ultimaEjecucion = NULL;
    int maxY, maxX;
    getmaxyx(ventana, maxY, maxX);
    int anchoTabla = maxX - 2;
    int desp = 1;                           /* la tabla empieza en la columna 1 */

    if (ultimaEjecucion != ejecucion) {
        fila = 4;
        ultimaEjecucion = ejecucion;
    }
    if (fila >= maxY - 1) fila = 4;

    imprimirNumero(ventana, fila, desp, anchoTabla, COL_ID,     ejecucion->id);
    imprimirCelda (ventana, fila, desp, anchoTabla, COL_NOMBRE, ejecucion->nombre, 0);
    imprimirNumero(ventana, fila, desp, anchoTabla, COL_PC,     ejecucion->PC);
    imprimirNumero(ventana, fila, desp, anchoTabla, COL_AX,     reg->EAX);
    imprimirNumero(ventana, fila, desp, anchoTabla, COL_BX,     reg->EBX);
    imprimirNumero(ventana, fila, desp, anchoTabla, COL_CX,     reg->ECX);
    imprimirNumero(ventana, fila, desp, anchoTabla, COL_DX,     reg->EDX);
    imprimirCelda (ventana, fila, desp, anchoTabla, COL_IR,     ejecucion->IR->texto, 0);
    imprimirCelda (ventana, fila, desp, anchoTabla, COL_STATUS, mensajeEstatus(resultado), 0);

    fila++;
    wrefresh(ventana);
}
