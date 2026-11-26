#include "prototipos.h"

/*
 * largoPlazo: mientras no existan los recursos, pasa todo Nuevos a Listos
 * para que el sistema se comporte igual que antes de esta fase.
 */

void largoPlazo(Cola *nuevos, Cola *listos)
{
    PCB *p;
    while ((p = extraer(nuevos)) != NULL) {
        p->estado = ESPERA;
        encolar(listos, p);
    }
}

void revisarBloqueados(Cola *bloqueados, Cola *listos)
{
    (void)bloqueados; (void)listos; /* TODO: Entrega 3 */
}

int matarProceso(int id, PCB **ejecucion, Cola *listos, Cola *terminados)
{
    (void)id; (void)ejecucion; (void)listos; (void)terminados;
    return PROCESO_NO_EXISTE;       /* TODO: Entrega 1 */
}
