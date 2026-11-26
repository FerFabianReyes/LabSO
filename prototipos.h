#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <curses.h>
#include <sys/select.h>
#include "estructuras.h"

#define FILA_INICIAL  4
#define TAM_COMANDO   256
#define MAX_ARCHIVOS  16

/*---------- PANTALLA ---------------------------------------------*/
/* nuevos y bloqueados son NULL hasta que Interfaz las cree; impNuevos,
   impBloqueados e impRecursos deben aceptar una ventana NULL. */
typedef struct {
    WINDOW *ejecucion, *nuevos, *listos, *bloqueados, *terminados, *comandos;
} Pantalla;

/*---------- ARCHIVOS (archivos.c) --------------------------------*/
Archivo* crearArchivo();
Renglon* crearRenglon(char *texto); 
void agregarRenglon(Archivo *archivo, char *texto); 
void liberarArchivo(Archivo *archivo);
int leerArchivo(char *nomArchivo, Archivo *archivo); 
PCB* crearPCB(Archivo *prog, char *nombre);
void liberarPCB(PCB *ejec); 
int cargarArchivo(char *nombre, Cola *nuevos, Cola *terminados);  /* BIEN o error; encola en Nuevos */

/*---------- COLAS Y CONTEXTO (colas.c) --------------------------------*/
Cola* crearCola(void);
void  encolar(Cola *c, PCB *p);        /* deja p->siguiente = NULL */
PCB*  extraer(Cola *c);                /* NULL si está vacía */
int   colaVacia(Cola *c);
void  liberarCola(Cola *c);
void  cargarContexto(PCB *p);          /* *reg  <- p->registro */
void  guardarContexto(PCB *p);         /* p->registro <- *reg */
PCB*  extraerPorId(Cola *c, int id);   /* saca de cualquier posicion; NULL si no esta */
PCB*  buscarPorId(Cola *c, int id);    /* sin sacarlo; NULL si no esta */

/*---------- COMANDOS (comandos.c) --------------------------------*/
int detectarComando(char cad[]);
int separarArchivos(char cad[], char *nombres[], int max);  /* regresa cuántos nombres */
int idKill(char cad[]);                /* id de "kill N"; -1 si no es valido */
void leerComando(WINDOW *ventana, int *pos, char cad[], int caracter);
void borrarCaracter(WINDOW *ventana, int *pos);
void imprimirCaracter(WINDOW *ventana, int *pos, char cad[], int caracter);

/*---------- INSTRUCCIONES (instrucciones.c) --------------------------------*/
Registros* crearRegistro();
int ejecutarPrograma(PCB *ejec);
int obtenerRegistro(char nombre[], Registros *registros, int **registro);
int obtenerValor(Token *tok);
int mov(Token *param);
int add(Token *param); 
int sub(Token *param);
int mul(Token *param);
int divR(Token *param);
int inc(Token *param);
int dec(Token *param);

/*---------- LEXICO Y SINTACTICO (lexicoSintactico.c) --------------------------------*/
int esNumero(Token *token);
int esRegistro(Token *token);  
int esInstruccion(Token *token); 
int verifSintaxis(Archivo *archivo); 
int parserDosParametros(Token *token); 
int parserUnParametro(Token *token); 
int extraerNumeros(Renglon *ren, int out[NUM_RECURSOS]);  /* MAX y GET; BIEN o error */
int espaciosMultiples(char *texto); 
int verifEspacios(char *texto);

/*---------- LOG ERRORES (logErrores.c) --------------------------------*/
const char *mensajeEstatus(int codigo); 
void mostrarErroresCarga(WINDOW *cmd, char *nombres[], int errores[], int n);
void mostrarErrorComando(WINDOW *cmd, int error);
void mostrarErrorKill(WINDOW *cmd, int id);   /* "Proceso con id: N no existe" */

/*---------- TOKENS (tokens.c) --------------------------------*/
int tokenizar(Archivo *archivo);  
void agregarToken(Renglon *renglon, Token *token); 
Token* crearToken();  
int agregarTipoDato(Token *token); 
void liberarRenglon(Renglon *renglon); 

/*---------- VENTANAS (ventanas.c) --------------------------------*/
int kbhit(void);
WINDOW *crearVentana(int altura, int anchura, int posY, char *nombre); 
void impVentanaComandos(WINDOW *ventana); 
void limpiarComando(WINDOW *ventana); 
void impEncabezado(WINDOW *ventana); 
void impEjecucion(WINDOW *ventana, PCB *proceso, int resultado); 
void impListos(WINDOW *v, Cola *c);        /* IR y Status en "-" */
void impTerminados(WINDOW *v, Cola *c);    /* últimos valores y última instrucción */
void imprimirCola(WINDOW *ventana, Cola *cola, int terminados);
void imprimirFilaPCB(WINDOW *ventana, int fila, int anchoTabla, PCB *p, const char *ir, const char *status);
void limpiarFilas(WINDOW *ventana);
Pantalla crearPantalla(void);          /* crea ventanas, encabezados y limpia Comandos */
void impNuevos(WINDOW *v, Cola *c);
void impBloqueados(WINDOW *v, Cola *c);
void impRecursos(WINDOW *v);           /* "Recursos: X=8 Y=8 Z=8" */

/*---------- RECURSOS (recursos.c) --------------------------------*/
void recursosIniciar(void);                              /* recursos[i] = LIMITE_RECURSOS */
int  recursosAlcanzan(const int pedido[NUM_RECURSOS]);   /* 1 si pedido <= recursos */
void recursosOtorgar(PCB *p, const int pedido[NUM_RECURSOS]);
void recursosLiberarProceso(PCB *p);                     /* devuelve p->usado al sistema */

/*---------- PLANIFICADOR (planificador.c) --------------------------------*/
void largoPlazo(Cola *nuevos, Cola *listos);             /* Nuevos -> Listos si alcanzan */
void revisarBloqueados(Cola *bloqueados, Cola *listos);  /* Bloqueados -> Listos si alcanzan */
int  matarProceso(int id, PCB **ejecucion, Cola *listos, Cola *terminados);  /* BIEN o PROCESO_NO_EXISTE */

#endif