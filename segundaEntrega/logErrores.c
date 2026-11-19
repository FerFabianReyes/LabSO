#include "prototipos.h"

const char *mensajeEstatus(int codigo)
{
    switch (codigo)
    {
    case BIEN:                      return "OK";
    case NOMBRE_INCORRECTO:         return "no encontrado";
    case NO_HAY_ARCHIVO:            return "no se pudo abrir";
    case NO_HAY_TEXTO:              return "vacío";
    case COMANDO_INVALIDO:          return "comando inválido";
    case LINEA_VACIA:               return "Linea vacia";
    case INSTRUCCION_NO_RECONOCIDA: return "Op. desconocida";
    case DIV_ENTRE_CERO:            return "Div. entre 0";

    case TIPO_PARAM_INVALIDO:
    case INICIA_MINUSCULA:
    case REGISTRO_INVALIDO:
    case NUM_DECIMAL:               return "Error lexico";

    case INSTR_REG_INCUMPLIDA:
    case PARAMETROS_INCORRECTOS:
    case PARAMETROS_INSUFICIENTES:
    case PARAMETROS_EXTRA:
    case ESPACIOS_EXTRA:
    case COMA_CON_ESPACIOS:         return "Error sintaxis";

    default:                        return "Error";
    }
}

/* Limpia la fila de salida y escribe un mensaje limitado al ancho visible. */
static void mostrarLineaComandos(WINDOW *cmd, const char *mensaje)
{
    if (!cmd) { return; }

    int maxY, maxX;
    getmaxyx(cmd, maxY, maxX);
    if (maxY < 2 || maxX < 3) { return; }

    wmove(cmd, 1, 1);
    wclrtoeol(cmd);
    if (mensaje) {
        mvwprintw(cmd, 1, 1, "%.*s", maxX - 2, mensaje);
    }
    box(cmd, 0, 0);                        /* wclrtoeol borró el borde derecho */
    mvwprintw(cmd, 0, 2, " Comandos ");    /* box tapa el título */
    wrefresh(cmd);
}

/* Muestra solo los archivos fallidos, conservando su orden de entrada. */
void mostrarErroresCarga(WINDOW *cmd, char *nombres[], int errores[], int n)
{
    if (!cmd) { return; }

    int maxY, maxX;
    getmaxyx(cmd, maxY, maxX);
    if (maxY < 2 || maxX < 3) { return; }

    char linea[4096] = "";
    size_t usados = 0;
    for (int i = 0; i < n; i++) {
        if (!errores || errores[i] == BIEN) { continue; }
        const char *nombre = (nombres && nombres[i]) ? nombres[i] : "(sin nombre)";
        const char *detalle = mensajeEstatus(errores[i]);
        int escritos = snprintf(linea + usados, sizeof(linea) - usados,
                                "%s%s: %s",
                                usados ? " | " : "", nombre,
                                detalle ? detalle : "Error");
        if (escritos < 0 || (size_t)escritos >= sizeof(linea) - usados) {
            usados = sizeof(linea) - 1;
            break;
        }
        usados += (size_t)escritos;
    }

    mostrarLineaComandos(cmd, linea);
}

/* Muestra el error de comando en la misma fila de salida de Comandos. */
void mostrarErrorComando(WINDOW *cmd, int error)
{
    char linea[128];
    snprintf(linea, sizeof(linea), "Comando: %s", mensajeEstatus(error));
    mostrarLineaComandos(cmd, linea);
}