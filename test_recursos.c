
#include <stdio.h>
#include <assert.h>
#include "prototipos.h"

int recursos[NUM_RECURSOS];

int main(void)
{
    PCB proceso = {0};

    int pedido[NUM_RECURSOS] = {3, 2, 4};
    int pedidoExcesivo[NUM_RECURSOS] = {3, 2, 9};
    int pedidoMaximo[NUM_RECURSOS] = {8, 8, 8};

    // Prueba 1: Inicializar recursos
    recursosIniciar();

    for (int i = 0; i < NUM_RECURSOS; i++)
    {
        assert(recursos[i] == LIMITE_RECURSOS);
    }

    printf("Prueba 1 correcta\n");

    // Prueba 2: Comprobar disponibilidad
    assert(recursosAlcanzan(pedido) == 1);
    assert(recursosAlcanzan(pedidoExcesivo) == 0);
    assert(recursosAlcanzan(pedidoMaximo) == 1);

    printf("Prueba 2 correcta\n");

    // Prueba 3: Otorgar recursos
    recursosOtorgar(&proceso, pedido);

    for (int i = 0; i < NUM_RECURSOS; i++)
    {
        assert(recursos[i] == LIMITE_RECURSOS - pedido[i]);
        assert(proceso.usado[i] == pedido[i]);
    }

    assert(recursosAlcanzan(pedidoMaximo) == 0);

    printf("Prueba 3 correcta\n");

    // Prueba 4: Liberar recursos
    recursosLiberarProceso(&proceso);

    for (int i = 0; i < NUM_RECURSOS; i++)
    {
        assert(recursos[i] == LIMITE_RECURSOS);
        assert(proceso.usado[i] == 0);
    }

    printf("Prueba 4 correcta\n");

    // Prueba 5: Liberar por segunda vez
    recursosLiberarProceso(&proceso);

    for (int i = 0; i < NUM_RECURSOS; i++)
    {
        assert(recursos[i] == LIMITE_RECURSOS);
        assert(proceso.usado[i] == 0);
    }

    printf("Prueba 5 correcta\n");

    printf("Todas las pruebas de recursos pasaron\n");

    return 0;
}
