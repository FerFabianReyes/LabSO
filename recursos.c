#include "prototipos.h"

/*
 * `recursos` (disponibles del sistema) se define en main.c.
 */

void recursosIniciar(void)
{
    for (int i = 0; i < NUM_RECURSOS; i++) { recursos[i] = LIMITE_RECURSOS; }
}

int recursosAlcanzan(const int pedido[NUM_RECURSOS])
{
    (void)pedido;
    return 1;                       /* TODO: pedido[i] <= recursos[i] para todo i */
}

void recursosOtorgar(PCB *p, const int pedido[NUM_RECURSOS])
{
    (void)p; (void)pedido;          /* TODO: recursos[i] -= pedido[i]; p->usado[i] += pedido[i] */
}

void recursosLiberarProceso(PCB *p)
{
    (void)p;                        /* TODO: recursos[i] += p->usado[i]; p->usado[i] = 0 */
}
