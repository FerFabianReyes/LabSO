#include "prototipos.h"

/* Regresa 1 si el texto coincide cn el patron (regex extendida) */
static int coincide(const char *texto, const char *patron)
{
    regex_t regex;
    if (regcomp(&regex, patron, REG_EXTENDED | REG_NOSUB) != 0) { return 0; }
    int resultado = regexec(&regex, texto, 0, NULL, 0);
    regfree(&regex);
    return resultado == 0;
}

/*--------------- Lexico ----------------------*/
int esNumero(Token *token)
{
    char *dato = token->textoToken;

    if (coincide(dato, "^[+-]?[0-9]+$")) { return NUM; }
    if (coincide(dato, "^[+-]?[0-9]*\\.[0-9]*$")) { return NUM_DECIMAL; }
    return TIPO_PARAM_INVALIDO;
}

int esRegistro(Token *token)
{
    char *dato = token->textoToken;

    if (coincide(dato, "^(AX|BX|CX|DX)$")) { return REG; }

    if (coincide(dato, "^(AX|BX|CX|DX),$")) {
        char *coma = strchr(dato, ',');
        *coma = '\0';
        return REG_COM;
    }

    if (coincide(dato, "^(ax|bx|cx|dx|Ax|Bx|Cx|Dx)$")) { return REGISTRO_INVALIDO; }
    return esNumero(token);
}

int esInstruccion(Token *token)
{
    char *dato = token->textoToken;

    if (coincide(dato, "^(MOV|ADD|SUB|MUL|DIV|INC|DEC|NEG|MOD|END)$")) { return INSTR; }
    if (coincide(dato, "^[a-z]+$")) { return INICIA_MINUSCULA; }
    return esRegistro(token);
}

/*--------------- Sintactico ----------------------*/

int parserDosParametros(Token *token)
{
    if (!token->sig) { return PARAMETROS_INSUFICIENTES; }

    Token *tem = token->sig;
    if (tem->tipoParam != REG_COM) { return INSTR_REG_INCUMPLIDA; }
    if (!tem->sig) { return PARAMETROS_INSUFICIENTES; }
    if (tem->sig->sig) { return PARAMETROS_EXTRA; }

    if (tem->sig->tipoParam == REG || tem->sig->tipoParam == NUM) { return BIEN; }
    return PARAMETROS_INCORRECTOS;
}

int parserUnParametro(Token *token)
{
    if (!token->sig) { return PARAMETROS_INSUFICIENTES; }
    if (token->sig->sig) { return PARAMETROS_EXTRA; }
    if (token->sig->tipoParam == REG) { return BIEN; }
    return PARAMETROS_INCORRECTOS;
}

/* Revisa la sintazis de un solo renglon */
static int verifRenglon(Renglon *ren)
{
    Token *tok = ren->primerToken;
    if (!tok) { return LINEA_VACIA; }
    if (tok->tipoParam != INSTR) { return INSTRUCCION_NO_RECONOCIDA; }

    char *op = tok->textoToken;
    if (!strcmp(op, "MOV") || !strcmp(op, "ADD") || !strcmp(op, "SUB") ||
        !strcmp(op, "MUL") || !strcmp(op, "DIV") || !strcmp(op, "MOD")) {
        return parserDosParametros(tok);
    }
    if (!strcmp(op, "INC") || !strcmp(op, "DEC") || !strcmp(op, "NEG")) {
        return parserUnParametro(tok);
    }
    if (!strcmp(op, "END"))
    {
        if (ren->sig) { return PARAMETROS_EXTRA; }
        return BIEN;
    }
    
    return INSTRUCCION_NO_RECONOCIDA;
}

int verifSintaxis(Archivo *archivo)
{
    if (!archivo) { return NO_HAY_ARCHIVO; }
    if (!archivo->inicio) { return NO_HAY_TEXTO; }

    for (Renglon *ren = archivo->inicio; ren; ren = ren->sig) {
        if (ren->error == BIEN) { ren->error = verifRenglon(ren); }
    }
    return BIEN;
}

int verifEspacios(char *texto)
{
    int len = strlen(texto);

    if (strchr(texto, '\t')) { return ESPACIOS_EXTRA; }
    if (texto[0] == ' ' || texto[len - 1] == ' ') { return ESPACIOS_EXTRA; }
    if (espaciosMultiples(texto) == ESPACIOS_EXTRA) { return ESPACIOS_EXTRA; }

    for (int i = 0; i < len; i++) {
        if (texto[i] == ',' &&
            ((i > 0 && texto[i - 1] == ' ') || texto[i + 1] == ' ')) {
            return COMA_CON_ESPACIOS;
        }
    }
    return BIEN;
}

int espaciosMultiples(char *texto)
{
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == ' ' && texto[i+1] == ' ') {
            return ESPACIOS_EXTRA;
        }
    }
    return BIEN;
}
