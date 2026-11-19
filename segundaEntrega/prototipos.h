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

/*---------- ARCHIVOS (archivos.c) --------------------------------*/
Archivo* crearArchivo();
Renglon* crearRenglon(char *texto); 
void agregarRenglon(Archivo *archivo, char *texto); 
void liberarArchivo(Archivo *archivo);
int leerArchivo(char *nomArchivo, Archivo *archivo); 
PCB* crearPCB(Archivo *prog, char *nombre);
void liberarPCB(PCB *ejec); 
int cargarArchivo(char *nombre, Cola *listos);   /* BIEN o error; si falla, no encola y libera lo creado */

/*---------- COLAS Y CONTEXTO (colas.c) --------------------------------*/
Cola* crearCola(void);
void  encolar(Cola *c, PCB *p);        /* deja p->siguiente = NULL */
PCB*  extraer(Cola *c);                /* NULL si está vacía */
int   colaVacia(Cola *c);
void  liberarCola(Cola *c);
void  cargarContexto(PCB *p);          /* *reg  <- p->registro */
void  guardarContexto(PCB *p);         /* p->registro <- *reg */

/*---------- COMANDOS (comandos.c) --------------------------------*/
int detectarComando(char cad[]);
int separarArchivos(char cad[], char *nombres[], int max);  /* regresa cuántos nombres */
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
int espaciosMultiples(char *texto); 
int verifEspacios(char *texto);

/*---------- LOG ERRORES (logErrores.c) --------------------------------*/
const char *mensajeEstatus(int codigo); 
void mostrarErroresCarga(WINDOW *cmd, char *nombres[], int errores[], int n);
void mostrarErrorComando(WINDOW *cmd, int error);

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

#endif