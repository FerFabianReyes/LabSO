#include "estructuras.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Cola *crearCola(void);
void encolar(Cola *cola, PCB *proceso);
PCB *extraer(Cola *cola);
int colaVacia(Cola *cola);
void liberarCola(Cola *cola);
void cargarContexto(PCB *proceso);
void guardarContexto(PCB *proceso);
#include <assert.h>

Registros *reg = NULL;

static int pcb_liberados = 0;

/*liberación del PCB completo. */
void liberarPCB(PCB *p)
{
    if (!p) return;
    free(p->registro);
    free(p->nombre);
    free(p);
    pcb_liberados++;
}

static PCB *crearPCBPrueba(int id)
{
    PCB *p = calloc(1, sizeof(*p));
    assert(p != NULL);

    p->registro = calloc(1, sizeof(*p->registro));
    assert(p->registro != NULL);

    p->nombre = malloc(16);
    assert(p->nombre != NULL);
    snprintf(p->nombre, 16, "proceso%d.asm", id);

    p->id = id;
    return p;
}

int main(void)
{
    reg = calloc(1, sizeof(*reg));
    assert(reg != NULL);

    Cola *cola = crearCola();
    assert(cola != NULL);
    assert(colaVacia(cola));
    assert(extraer(cola) == NULL);

    PCB *p1 = crearPCBPrueba(1);
    PCB *p2 = crearPCBPrueba(2);

    p1->registro->EAX = 10;
    p1->registro->EBX = 20;
    p1->PC = 7;

    encolar(cola, p1);
    encolar(cola, p2);
    assert(cola->tamanio == 2);
    assert(!colaVacia(cola));

    /* FIFO: sale primero el primer PCB insertado. */
    PCB *actual = extraer(cola);
    assert(actual == p1);
    assert(actual->siguiente == NULL);
    assert(cola->tamanio == 1);

    /* Carga de contexto PCB -> CPU. */
    reg->EAX = -1;
    reg->EBX = -1;
    cargarContexto(actual);
    assert(reg->EAX == 10);
    assert(reg->EBX == 20);

    /* Guardado de contexto CPU -> PCB. */
    reg->EAX = 99;
    reg->ECX = 33;
    guardarContexto(actual);
    assert(actual->registro->EAX == 99);
    assert(actual->registro->ECX == 33);

    /* Devuelve el segundo PCB y luego NULL. */
    assert(extraer(cola) == p2);
    assert(colaVacia(cola));
    assert(extraer(cola) == NULL);

    liberarPCB(actual);
    liberarPCB(p2);
    free(cola);
    assert(pcb_liberados == 2);

    /*liberarCola libera los PCB que conserva. */
    cola = crearCola();
    assert(cola != NULL);
    encolar(cola, crearPCBPrueba(3));
    encolar(cola, crearPCBPrueba(4));
    liberarCola(cola);
    assert(pcb_liberados == 4);

    free(reg);
    reg = NULL;

    puts("Pruebas de colas y contexto: correctas");
    return 0;
}
