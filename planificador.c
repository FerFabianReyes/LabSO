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

/*
 * kill: busca el proceso en Ejecucion y en Listos. Si lo encuentra lo manda a
 * Terminados con estatusFinal = KILLED y los registros calculados hasta ahora.
 * Regresa BIEN o PROCESO_NO_EXISTE.
 * TODO Entrega 3: liberar sus recursos (recursosLiberarProceso).
 */
int matarProceso(int id, PCB **ejecucion, Cola *listos, Cola *terminados)
{
    PCB *objetivo = NULL;

    if (ejecucion && *ejecucion && (*ejecucion)->id == id) {
        objetivo = *ejecucion;
        *ejecucion = NULL;
        guardarContexto(objetivo);          /* reg tiene los valores actuales */
    } else {
        objetivo = extraerPorId(listos, id);
    }

    if (!objetivo) {
        return PROCESO_NO_EXISTE;
    }

    objetivo->estado = TERMINADO;
    objetivo->estatusFinal = KILLED;
    objetivo->ejecutadas = 0;
    encolar(terminados, objetivo);
    return BIEN;
}
