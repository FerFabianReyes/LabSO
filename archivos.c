#include "prototipos.h"

PCB* crearPCB(Archivo *prog, char *nombre)
{
    PCB *nueva = malloc(sizeof(PCB));

    char *base = nombre ? strrchr(nombre, '/') : NULL;
    nueva->nombre = strdup(base ? base + 1 : (nombre ? nombre : ""));

    nueva->registro = crearRegistro();

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
    nuevo->final = NULL;
    nuevo->inicio = NULL;
    nuevo->tamanio = 0;
    return nuevo;
}

Renglon* crearRenglon(char *texto)
{
    Renglon *nuevo = malloc(sizeof(Renglon));
    nuevo->texto = strdup(texto);
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
    Renglon *nuevo = crearRenglon(texto);
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

    while (getline(&ren, &tamanio, arch) != -1) {
        agregarRenglon(archivo, ren);
    }

    fclose(arch);
    free(ren);
    return BIEN;
}
