#include "prototipos.h"

Token* crearToken()
{
    Token *nuevo = malloc(sizeof(Token));
    nuevo->textoToken = NULL;
    nuevo->sig = NULL;
    nuevo->tipoParam = 0;
    return nuevo;
}

void agregarToken(Renglon *renglon, Token *token)
{
    if (!renglon->primerToken) {
        renglon->primerToken = token;
    } else {
        renglon->ultimoToken->sig = token;
    }
    renglon->ultimoToken = token;
}

int agregarTipoDato(Token *token)
{
    int tipoDato = esInstruccion(token);

    if (tipoDato == INSTR || tipoDato == REG || tipoDato == REG_COM || tipoDato == NUM) {
        token->tipoParam = tipoDato;
        return BIEN;
    }
    return tipoDato;
}

void liberarRenglon(Renglon *renglon)
{
    Token *tokenActual = renglon->primerToken;
    while (tokenActual) {
        Token *tempToken = tokenActual;
        tokenActual = tokenActual->sig;
        free(tempToken->textoToken);
        free(tempToken);
    }
    free(renglon->texto);
    free(renglon);
}

void quitarComentario(Renglon *ren)
{
    char *com  = strchr(ren->texto, ';');
    if (com) {
        *com = '\0';
        int n = com - ren->texto;
        while (n > 0 && ren->texto[n-1] == ' ')
        {
            ren->texto[--n] = '\0';
        }
    }
    
}

/* Separa un renglin en tokens. Regresa BIEN o el error encontrado. */
static int tokenizarRenglon(Renglon *ren)
{
    size_t len = strlen(ren->texto);

    /* Renglon vacio o solo con espacios */
    if (strspn(ren->texto, " \t") == len) { return LINEA_VACIA; }

    quitarComentario(ren);

    int resEspacios = verifEspacios(ren->texto);
    if (resEspacios != BIEN) { return resEspacios; }

    char *textoCopia = malloc(len * 2 + 1);
    int j = 0;
    for (size_t i = 0; i < len; i++) {
        textoCopia[j++] = ren->texto[i];
        if (ren->texto[i] == ',' && ren->texto[i + 1] != '\0') {
            textoCopia[j++] = ' ';
        }
    }
    textoCopia[j] = '\0';

    int res = BIEN;
    char *delim;
    char *palabra = strtok_r(textoCopia, " \t", &delim);

    while (palabra) {
        Token *nuevoToken = crearToken();
        nuevoToken->textoToken = strdup(palabra);
        res = agregarTipoDato(nuevoToken);

        if (res != BIEN) {
            /* Si el primer token no es valid es una operacion desconocida  */
            if (res == TIPO_PARAM_INVALIDO && !ren->primerToken) {
                res = INSTRUCCION_NO_RECONOCIDA;
            }
            free(nuevoToken->textoToken);
            free(nuevoToken);
            break;
        }
        agregarToken(ren, nuevoToken);
        palabra = strtok_r(NULL, " \t", &delim);
    }

    free(textoCopia);
    return res;
}

int tokenizar(Archivo *archivo)
{
    if (!archivo) { return NO_HAY_ARCHIVO; }

    for (Renglon *temp = archivo->inicio; temp; temp = temp->sig) {
        temp->error = tokenizarRenglon(temp);
    }
    return BIEN;
}
