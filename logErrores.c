#include "prototipos.h"

void detectarError(WINDOW *ventana, int error)
{
    limpiarVentana(ventana, " Errores ");
    switch (error)
    {
    case BIEN:
        break;
    /*-- Archivos --*/
    case NOMBRE_INCORRECTO:
        mvwprintw(ventana, 2, 1, " Error %d: Archivo no encontrado. Favor de verificar el nombre", error);
        break;
    case NO_HAY_ARCHIVO:
        mvwprintw(ventana, 2, 1, " Error %d: No se pudo abrir el archivo", error);
        break;
    case NO_HAY_TEXTO:
        mvwprintw(ventana, 2, 1, " Error %d: El archivo está vacío", error);
        break;

    /*-- Tokens --*/
    case NO_HAY_TOKENS:
        mvwprintw(ventana, 2, 1, " Error %d: No se encontraron tokens", error);
        break;
    case LINEA_VACIA:
        mvwprintw(ventana, 2, 1, " Error %d: Se encontró una línea vacía", error);
        break;

    /*-- Léxico --*/
    case TIPO_PARAM_INVALIDO:
        mvwprintw(ventana, 2, 1, " Error %d: Los parámetros que se ingresaron son inválidos, favor de verificar", error);
        break;
    case INICIA_MINUSCULA:
        mvwprintw(ventana, 2, 1, " Error %d: No se permiten instrucciones ni parámetros en minúsculas", error);
        break;
    case REGISTRO_INVALIDO:
        mvwprintw(ventana, 2, 1, " Error %d: Sólo se permiten los registros AX, BX, CX y DX en mayúsculas", error);
        break;
    case NUM_DECIMAL:
        mvwprintw(ventana, 2, 1, " Error %d: No se permite ingresar números decimales", error);
        break;

    /*-- Sintáctico --*/
    case INSTR_REG_INCUMPLIDA:
        mvwprintw(ventana, 2, 1, " Error %d: Revise los parámetros después de las instrucciones", error);
        break;
    case PARAMETROS_INCORRECTOS:
        mvwprintw(ventana, 2, 1, " Error %d: Se detectaron instrucciones con parámetros incorrectos", error);
        break;
    case PARAMETROS_INSUFICIENTES:
        mvwprintw(ventana, 2, 1, " Error %d: Se detectaron instrucciones con parámetros insuficientes", error);
        break;
    case PARAMETROS_EXTRA:
        mvwprintw(ventana, 2, 1, " Error %d: Se detectaron instrucciones con parámetros extra", error);
        break;
    case DIV_ENTRE_CERO:
        mvwprintw(ventana, 2, 1, " Error %d: No se puede dividir entre cero", error);
        break;
    case ESPACIOS_EXTRA:
        mvwprintw(ventana, 2, 1, " Error %d: Se detectaron espacios múltiples en instrucciones", error);
        break;
    case INSTRUCCION_NO_RECONOCIDA:
        mvwprintw(ventana, 2, 1, " Error %d: Operación no reconocida", error);
        break;

    /*-- Comandos --*/
    case COMANDO_INVALIDO:
        mvwprintw(ventana, 2, 1, " Error %d: El comando que ingresó es inválido. Favor de verificar", error);
        break;

    default:
        mvwprintw(ventana, 2, 1, " Error %d: Error desconocido", error);
        break;
    }
    wrefresh(ventana);
}

const char *mensajeEstatus(int codigo)
{
    switch (codigo)
    {
    case BIEN:                      return "OK";
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
    case ESPACIOS_EXTRA:            return "Error sintaxis";

    default:                        return "Error";
    }
}
