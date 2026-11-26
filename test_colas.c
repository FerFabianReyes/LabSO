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
PCB *extraerPorId(Cola *cola, int id);
PCB *buscarPorId(Cola *cola, int id);
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

    /* buscarPorId / extraerPorId: primero, medio, ultimo y ausente. */
    cola = crearCola();
    PCB *a = crearPCBPrueba(10), *b = crearPCBPrueba(11), *c = crearPCBPrueba(12);
    encolar(cola, a); encolar(cola, b); encolar(cola, c);

    assert(buscarPorId(cola, 11) == b);
    assert(buscarPorId(cola, 99) == NULL);
    assert(buscarPorId(NULL, 10) == NULL);
    assert(cola->tamanio == 3);                 /* buscar no saca */

    assert(extraerPorId(cola, 99) == NULL);
    assert(extraerPorId(cola, 11) == b);        /* medio */
    assert(b->siguiente == NULL);
    assert(cola->tamanio == 2);
    assert(a->siguiente == c);

    assert(extraerPorId(cola, 12) == c);        /* ultimo: final debe actualizarse */
    assert(cola->final == a);
    encolar(cola, b);
    assert(cola->final == b && a->siguiente == b);

    assert(extraerPorId(cola, 10) == a);        /* primero */
    assert(cola->frente == b);
    assert(extraerPorId(cola, 11) == b);        /* unico: la cola queda vacia */
    assert(colaVacia(cola) && cola->final == NULL && cola->tamanio == 0);

    liberarPCB(a); liberarPCB(b); liberarPCB(c);
    free(cola);

    free(reg);
    reg = NULL;

    puts("Pruebas de colas y contexto: correctas");
    return 0;
}
