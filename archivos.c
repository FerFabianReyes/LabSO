#include "prototipos.h"

PCB* crearPCB(Archivo *prog, char *nombre)
{
    if (!prog) { return NULL; }

    PCB *nueva = calloc(1, sizeof(PCB));
    if (!nueva) { return NULL; }

    char *base = nombre ? strrchr(nombre, '/') : NULL;
    nueva->nombre = strdup(base ? base + 1 : (nombre ? nombre : ""));
    nueva->registro = crearRegistro();

    if (!nueva->nombre || !nueva->registro) {
        free(nueva->nombre);
        free(nueva->registro);
        free(nueva);
        return NULL;
    }

    nueva->programa = prog;
    nueva->PC = 0;
    nueva->IR = prog->inicio;
    nueva->estado = ESPERA;
    nueva->espera = 0;
    static int siguienteId = 0;
    nueva->id = siguienteId++;
    nueva->siguiente = NULL;
    return nueva;
}
 
Archivo* crearArchivo()
{
    Archivo *nuevo = malloc(sizeof(Archivo));
    if (!nuevo) { return NULL; }
    nuevo->final = NULL;
    nuevo->inicio = NULL;
    nuevo->tamanio = 0;
    return nuevo;
}

Renglon* crearRenglon(char *texto)
{
    Renglon *nuevo = malloc(sizeof(Renglon));
    if (!nuevo) { return NULL; }
    nuevo->texto = strdup(texto);
    if (!nuevo->texto) { free(nuevo); return NULL; }
    /* Quita \n y \r  */
    nuevo->texto[strcspn(nuevo->texto, "\r\n")] = '\0';
    nuevo->error = BIEN;
    nuevo->sig = NULL;
    nuevo->primerToken = NULL;
    nuevo->ultimoToken = NULL;
    return nuevo;
}

void agregarRenglon(Archivo *archivo, char *texto)
{
    if (!archivo || !texto) { return; }
    Renglon *nuevo = crearRenglon(texto);
    if (!nuevo) { return; }
    if (archivo->final) {
        archivo->final->sig = nuevo;
    } else {
        archivo->inicio = nuevo;
    }
    archivo->final = nuevo;
    archivo->tamanio++;
}

void liberarArchivo(Archivo *archivo)
{
    if (!archivo) { return; }
    Renglon *actual = archivo->inicio;
    while (actual) {
        Renglon *temp = actual;
        actual = actual->sig;
        liberarRenglon(temp);
    }
    free(archivo);
}

void liberarPCB(PCB *ejec)
{
    if (!ejec) { return; }
    liberarArchivo(ejec->programa);
    free(ejec->nombre);
    free(ejec->registro);
    ejec->registro = NULL;
    free(ejec);
}

int leerArchivo(char *nomArchivo, Archivo *archivo)
{
    if (!archivo) { return NO_HAY_ARCHIVO; }
    if (!nomArchivo) { return NOMBRE_INCORRECTO; }

    char *ren = NULL;
    size_t tamanio = 0;

    FILE *arch = fopen(nomArchivo, "r");
    if (arch == NULL) { return NOMBRE_INCORRECTO; }

    ssize_t leidos;
    while ((leidos = getline(&ren, &tamanio, arch)) != -1) {
        int anterior = archivo->tamanio;
        agregarRenglon(archivo, ren);
        if (archivo->tamanio == anterior) {
            free(ren);
            fclose(arch);
            return NO_HAY_ARCHIVO;
        }
    }

    int errorLectura = ferror(arch);
    fclose(arch);
    free(ren);
    return errorLectura ? NO_HAY_ARCHIVO : BIEN;
}

/* Carga transaccional: solo encola el PCB cuando todas las etapas validan. */
int cargarArchivo(char *nombre, Cola *listos)
{
    if (!nombre || !*nombre) { return NOMBRE_INCORRECTO; }
    if (!listos) { return NO_HAY_ARCHIVO; }

    Archivo *archivo = crearArchivo();
    if (!archivo) { return NO_HAY_ARCHIVO; }

    int resultado = leerArchivo(nombre, archivo);
    if (resultado == BIEN) { resultado = tokenizar(archivo); }
    if (resultado == BIEN) { resultado = verifSintaxis(archivo); }

    /* Los errores léxicos y sintácticos se almacenan por renglón. */
    if (resultado == BIEN) {
        for (Renglon *r = archivo->inicio; r; r = r->sig) {
            if (r->error != BIEN) {
                resultado = r->error;
                break;
            }
        }
    }

    if (resultado != BIEN) {
        liberarArchivo(archivo);
        return resultado;
    }

    PCB *proceso = crearPCB(archivo, nombre);
    if (!proceso) {
        liberarArchivo(archivo);
        return NO_HAY_ARCHIVO;
    }

    encolar(listos, proceso);
    return BIEN;
}
