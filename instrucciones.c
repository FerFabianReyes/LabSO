#include "prototipos.h"

Registros* crearRegistro()
{
    Registros *nuevo = malloc(sizeof(Registros));
    nuevo->EAX = 0;    nuevo->EBX = 0;
    nuevo->ECX = 0;    nuevo->EDX = 0;
    return nuevo;
}

int ejecutarPrograma(Ejecucion *ejec)
{
    Renglon *ren = ejec->IR;
    ejec->PC++;

    if (ren->error != BIEN) { return ren->error; }

    Token *instr = ren->primerToken;
    char *op = instr->textoToken;

    if (!strcmp(op, "MOV")) { return mov(instr); }
    if (!strcmp(op, "ADD")) { return add(instr); }
    if (!strcmp(op, "SUB")) { return sub(instr); }
    if (!strcmp(op, "MUL")) { return mul(instr); }
    if (!strcmp(op, "DIV")) { return divR(instr); }
    if (!strcmp(op, "INC")) { return inc(instr); }
    if (!strcmp(op, "DEC")) { return dec(instr); }

    return INSTRUCCION_NO_RECONOCIDA;
}

int *obtenerRegistro(char *registro)
{
    if (!strcmp(registro, "AX")) { return &reg->EAX; }
    if (!strcmp(registro, "BX")) { return &reg->EBX; }
    if (!strcmp(registro, "CX")) { return &reg->ECX; }
    if (!strcmp(registro, "DX")) { return &reg->EDX; }
    return NULL;
}

/* PENDOENTE mov, add, sub, mul y divR repiten
   el mismo bloque para obtener el valor del segundo parametro. */

int mov(Token *param)
{
    int *regDestino = obtenerRegistro(param->sig->textoToken);
    if (param->sig->sig->tipoParam == REG) {
        int *regValor = obtenerRegistro(param->sig->sig->textoToken);
        *regDestino = *regValor;
    } else {
        int num = atoi(param->sig->sig->textoToken);
        *regDestino = num;
    }
    return BIEN;
}

int add(Token *param)
{
    int *regDestino = obtenerRegistro(param->sig->textoToken);
    if (param->sig->sig->tipoParam == REG) {
        int *regValor = obtenerRegistro(param->sig->sig->textoToken);
        *regDestino = *regDestino + *regValor;
    } else {
        int num = atoi(param->sig->sig->textoToken);
        *regDestino = *regDestino + num;
    }
    return BIEN;
}

int sub(Token *param)
{
    int *regDestino = obtenerRegistro(param->sig->textoToken);
    if (param->sig->sig->tipoParam == REG) {
        int *regValor = obtenerRegistro(param->sig->sig->textoToken);
        *regDestino = *regDestino - *regValor;
    } else {
        int num = atoi(param->sig->sig->textoToken);
        *regDestino = *regDestino - num;
    }
    return BIEN;
}

int mul(Token *param)
{
    int *regDestino = obtenerRegistro(param->sig->textoToken);
    if (param->sig->sig->tipoParam == REG) {
        int *regValor = obtenerRegistro(param->sig->sig->textoToken);
        *regDestino = *regDestino * *regValor;
    } else {
        int num = atoi(param->sig->sig->textoToken);
        *regDestino = *regDestino * num;
    }
    return BIEN;
}

int divR(Token *param)
{
    int *regDestino = obtenerRegistro(param->sig->textoToken);
    if (param->sig->sig->tipoParam == REG) {
        int *regValor = obtenerRegistro(param->sig->sig->textoToken);
        if (!(*regValor)) { return DIV_ENTRE_CERO; }
        *regDestino = *regDestino / *regValor;
    } else {
        int num = atoi(param->sig->sig->textoToken);
        if (!num) { return DIV_ENTRE_CERO; }
        *regDestino = *regDestino / num;
    }
    return BIEN;
}

int inc(Token *param)
{
    int *regDestino = obtenerRegistro(param->sig->textoToken);
    *regDestino = *regDestino + 1;
    return BIEN;
}

int dec(Token *param)
{
    int *regDestino = obtenerRegistro(param->sig->textoToken);
    *regDestino = *regDestino - 1;
    return BIEN;
}
