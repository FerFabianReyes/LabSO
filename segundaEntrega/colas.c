#include "estructuras.h"
#include <stdlib.h>

void liberarPCB(PCB *proceso);

/*
 * Las colas reutilizan el campo siguiente del PCB como enlace.
 * Un PCB debe pertenecer a una sola cola a la vez.
 */

Cola *crearCola(void)
{
    Cola *cola = malloc(sizeof(*cola));
    if (!cola) {
        return NULL;
    }

    cola->frente = NULL;
    cola->final = NULL;
    cola->tamanio = 0;
    return cola;
}

void encolar(Cola *cola, PCB *proceso)
{
    if (!cola || !proceso) {
        return;
    }

    proceso->siguiente = NULL;

    if (cola->final) {
        cola->final->siguiente = proceso;
    } else {
        cola->frente = proceso;
    }

    cola->final = proceso;
    cola->tamanio++;
}

PCB *extraer(Cola *cola)
{
    if (!cola || !cola->frente) {
        return NULL;
    }

    PCB *proceso = cola->frente;
    cola->frente = proceso->siguiente;

    if (!cola->frente) {
        cola->final = NULL;
    }

    proceso->siguiente = NULL;
    if (cola->tamanio > 0) {
        cola->tamanio--;
    }

    return proceso;
}

int colaVacia(Cola *cola)
{
    return (!cola || cola->frente == NULL);
}

void liberarCola(Cola *cola)
{
    if (!cola) {
        return;
    }

    PCB *proceso;
    while ((proceso = extraer(cola)) != NULL) {
        liberarPCB(proceso);
    }

    free(cola);
}

/*
 * reg representa los registros actuales de la CPU.
 * El PCB conserva su propia copia independiente del contexto.
 */
void cargarContexto(PCB *proceso)
{
    if (!proceso || !proceso->registro || !reg) {
        return;
    }

    *reg = *proceso->registro;
}

void guardarContexto(PCB *proceso)
{
    if (!proceso || !proceso->registro || !reg) {
        return;
    }

    *proceso->registro = *reg;
}
