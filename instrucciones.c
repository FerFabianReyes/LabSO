#include "prototipos.h"

Registros *crearRegistro()
{
    Registros *nuevo = malloc(sizeof(Registros));
    nuevo->EAX = 0;
    nuevo->EBX = 0;
    nuevo->ECX = 0;
    nuevo->EDX = 0;
    return nuevo;
}

int ejecutarPrograma(Ejecucion *ejec)
{
    Renglon *ren = ejec->IR;
    ejec->PC++;

    if (ren->error != BIEN)
    {
        return ren->error;
    }

    Token *instr = ren->primerToken;
    char *op = instr->textoToken;

    if (!strcmp(op, "MOV"))
    {
        return mov(instr);
    }
    if (!strcmp(op, "ADD"))
    {
        return add(instr);
    }
    if (!strcmp(op, "SUB"))
    {
        return sub(instr);
    }
    if (!strcmp(op, "MUL"))
    {
        return mul(instr);
    }
    if (!strcmp(op, "DIV"))
    {
        return divR(instr);
    }
    if (!strcmp(op, "INC"))
    {
        return inc(instr);
    }
    if (!strcmp(op, "DEC"))
    {
        return dec(instr);
    }
    if (!strcmp(op, "NEG"))
    {
        return neg(instr);
    }
    if (!strcmp(op, "MOD"))
    {
        return mod(instr);
    }
    if (!strcmp(op, "END"))
    {
        ejec->estado = TERMINADO;
        return BIEN;
    }
    return INSTRUCCION_NO_RECONOCIDA;
}

int obtenerRegistro(char nombre[], Registros *registros, int **registro)
{
    if (strcmp(nombre, "AX") == 0)
    {
        *registro = &registros->EAX;
    }
    else if (strcmp(nombre, "BX") == 0)
    {
        *registro = &registros->EBX;
    }
    else if (strcmp(nombre, "CX") == 0)
    {
        *registro = &registros->ECX;
    }
    else if (strcmp(nombre, "DX") == 0)
    {
        *registro = &registros->EDX;
    }
    else
    {
        return 0;
    }

    return 1;
}

int obtenerValor(Token *tok)
{
    if (tok->tipoParam == REG)
    {
        int *registro;
        obtenerRegistro(tok->textoToken, reg, &registro);
        return *registro;
    }

    if (tok->tipoParam == NUM)
    {
        return atoi(tok->textoToken);
    }

    return 0;
}

int mov(Token *param)
{
    int *regDestino;
    obtenerRegistro(param->sig->textoToken, reg, &regDestino);

    *regDestino = obtenerValor(param->sig->sig);
    return BIEN;
}

int add(Token *param)
{
    int *regDestino;
    obtenerRegistro(param->sig->textoToken, reg, &regDestino);

    *regDestino += obtenerValor(param->sig->sig);
    return BIEN;
}

int sub(Token *param)
{
    int *regDestino;
    obtenerRegistro(param->sig->textoToken, reg, &regDestino);

    *regDestino -= obtenerValor(param->sig->sig);
    return BIEN;
}

int mul(Token *param)
{
    int *regDestino;
    obtenerRegistro(param->sig->textoToken, reg, &regDestino);

    *regDestino *= obtenerValor(param->sig->sig);
    return BIEN;
}

int divR(Token *param)
{
    int *regDestino;
    obtenerRegistro(param->sig->textoToken, reg, &regDestino);

    int divisor = obtenerValor(param->sig->sig);
    if (divisor == 0)
    {
        return DIV_ENTRE_CERO;
    }

    *regDestino /= divisor;
    return BIEN;
}

int mod(Token *param)
{
    int *regDestino;
    obtenerRegistro(param->sig->textoToken, reg, &regDestino);

    int divisor = obtenerValor(param->sig->sig);
    if (divisor == 0)
    {
        return DIV_ENTRE_CERO;
    }

    *regDestino %= divisor;
    return BIEN;
}

int inc(Token *param)
{
    int *regDestino;

    if (!obtenerRegistro(param->sig->textoToken, reg, &regDestino))
    {
        return REGISTRO_INVALIDO;
    }

    *regDestino += 1;
    return BIEN;
}

int dec(Token *param)
{
    int *regDestino;

    if (!obtenerRegistro(param->sig->textoToken, reg, &regDestino))
    {
        return REGISTRO_INVALIDO;
    }

    *regDestino -= 1;
    return BIEN;
}

int neg(Token *param)
{
    int *regDestino;

    if (!obtenerRegistro(param->sig->textoToken, reg, &regDestino))
    {
        return REGISTRO_INVALIDO;
    }

   *regDestino = *regDestino * -1;
    return BIEN;
}