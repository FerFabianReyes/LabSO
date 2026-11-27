
#include "prototipos.h"

void recursosIniciar(void)
{
    for (int i = 0; i < NUM_RECURSOS; i++)
    {
        recursos[i] = LIMITE_RECURSOS;
    }
}

int recursosAlcanzan(const int pedido[NUM_RECURSOS])
{
    for (int i = 0; i < NUM_RECURSOS; i++)
    {
        if (pedido[i] > recursos[i])
        {
            return 0;
        }
    }

    return 1;
}

void recursosOtorgar(PCB *p, const int pedido[NUM_RECURSOS])
{
    for (int i = 0; i < NUM_RECURSOS; i++)
    {
        recursos[i] -= pedido[i];
        p->usado[i] += pedido[i];
    }
}

void recursosLiberarProceso(PCB *p)
{
    for (int i = 0; i < NUM_RECURSOS; i++)
    {
        recursos[i] += p->usado[i];
        p->usado[i] = 0;
    }
}
