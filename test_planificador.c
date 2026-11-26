#include "prototipos.h"
#include <assert.h>

/* Prueba de matarProceso y del quantum sin curses. */

Registros *reg = NULL;
int Q = 4;
int recursos[NUM_RECURSOS];

void liberarPCB(PCB *p)
{
    if (!p) return;
    free(p->registro);
    free(p->nombre);
    free(p);
}

static PCB *crearPCBPrueba(int id)
{
    PCB *p = calloc(1, sizeof(*p));
    assert(p != NULL);
    p->registro = calloc(1, sizeof(*p->registro));
    assert(p->registro != NULL);
    p->id = id;
    p->estado = ESPERA;
    return p;
}

int main(void)
{
    reg = calloc(1, sizeof(*reg));
    assert(reg != NULL);

    Cola *listos = crearCola();
    Cola *terminados = crearCola();
    PCB *p1 = crearPCBPrueba(1);
    PCB *p2 = crearPCBPrueba(2);
    PCB *p3 = crearPCBPrueba(3);
    p3->registro->EAX = 5;          /* valor viejo, debe pisarse al matar */
    encolar(listos, p1);
    encolar(listos, p2);

    PCB *ejecucion = p3;
    reg->EAX = 42;                  /* registros "calculados hasta ahora" */
    reg->EBX = 7;

    /* id inexistente: nada cambia */
    assert(matarProceso(99, &ejecucion, listos, terminados) == PROCESO_NO_EXISTE);
    assert(ejecucion == p3 && listos->tamanio == 2 && colaVacia(terminados));

    /* matar el de Ejecucion: guarda contexto y libera la CPU */
    assert(matarProceso(3, &ejecucion, listos, terminados) == BIEN);
    assert(ejecucion == NULL);
    assert(p3->estado == TERMINADO && p3->estatusFinal == KILLED);
    assert(p3->registro->EAX == 42 && p3->registro->EBX == 7);
    assert(terminados->frente == p3);

    /* matar uno de Listos (el ultimo) */
    assert(matarProceso(2, &ejecucion, listos, terminados) == BIEN);
    assert(listos->tamanio == 1 && listos->frente == p1 && listos->final == p1);
    assert(p2->estatusFinal == KILLED && terminados->tamanio == 2);

    /* ya no existe en ninguna cola activa */
    assert(matarProceso(2, &ejecucion, listos, terminados) == PROCESO_NO_EXISTE);

    liberarCola(listos);
    liberarCola(terminados);
    free(reg);
    reg = NULL;

    puts("Pruebas de planificador (kill): correctas");
    return 0;
}
