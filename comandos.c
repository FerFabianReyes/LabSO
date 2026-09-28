#include "prototipos.h"

void borrarCaracter(WINDOW *ventana, int *pos)
{
    (*pos)--;
    mvwaddch(ventana, 1, (*pos)+4, ' ');
    wmove(ventana, 1, (*pos)+4);
    wrefresh(ventana);
}

void imprimirCaracter(WINDOW *ventana, int *pos, char cad[], int caracter)
{
    cad[*pos] = (char)caracter;
    mvwaddch(ventana, 1, (*pos)+4, caracter);
    wrefresh(ventana);
    (*pos)++;
}

void leerComando(WINDOW *ventana, int *pos, char cad[], int caracter)
{
    wmove(ventana, 1, (*pos)+4);
    wrefresh(ventana);

    if (caracter == KEY_BACKSPACE || caracter == 127) {
        if (*pos > 0) { borrarCaracter(ventana, pos); }
    } else if (caracter >= 32 && caracter < 127 && *pos < 48) {
        imprimirCaracter(ventana, pos, cad, caracter);
    }
}

int detectarComando(char cad[])
{
    /* Quita espacios al inicio y al final para que el comando no falle por un espacio extra. */
    char *inicio = cad;
    while (*inicio == ' ' || *inicio == '\t') inicio++;
    if (inicio != cad) memmove(cad, inicio, strlen(inicio) + 1);

    int n = strlen(cad);
    while (n > 0 && (cad[n-1] == ' ' || cad[n-1] == '\t')) cad[--n] = '\0';

    if (strcmp(cad, "salir") == 0) { return SALIR; }

    regex_t regex;
    char *patron = "^ejecutar[[:space:]]+[^[:space:]]+$";
    int resultado = regcomp(&regex, patron, REG_EXTENDED | REG_NOSUB);
    if (resultado != 0) return COMANDO_INVALIDO;
    resultado = regexec(&regex, cad, 0, NULL, 0);
    regfree(&regex);

    if (!resultado) { return EJECUTAR_ARCHIVO; }
    return COMANDO_INVALIDO;
}

char *sacarNomArchivo(char cad[])
{
    /* Salta todos los espacios  entre "ejecutar" y el nombre del archivo */
    char *nomArchivo = strpbrk(cad, " \t");
    if (!nomArchivo) { return NULL; }
    while (*nomArchivo == ' ' || *nomArchivo == '\t') nomArchivo++;
    return nomArchivo;
}
