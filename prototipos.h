#ifndef PROTOTIPOS_H
#define PROTOTIPOS_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include <curses.h>
#include <sys/select.h>
#include "estructuras.h"

/*---------- ARCHIVOS --------------------------------*/
Archivo* crearArchivo(); // F
Renglon* crearRenglon(char *texto); // J
void agregarRenglon(Archivo *archivo, char *texto); // I
void liberarArchivo(Archivo *archivo); // G
int leerArchivo(char *nomArchivo, Archivo *archivo); // F
PCB* crearPCB(Archivo *prog, char *nombre); //J
void liberarPCB(PCB *ejec); // I

/*---------- TOKENS --------------------------------*/
int tokenizar(Archivo *archivo); //G
void agregarToken(Renglon *renglon, Token *token); // F
Token* crearToken(); //J
int agregarTipoDato(Token *token);//I
void liberarRenglon(Renglon *renglon);//G

/*---------- LEXICO Y SINTACTICO --------------------------------*/
int esNumero(Token *token); //F ----------------
int esRegistro(Token *token); //J
int esInstruccion(Token *token);//I
int verifSintaxis(Archivo *archivo);//G
int parserDosParametros(Token *token);//F -----------------
int parserUnParametro(Token *token);//J
int espaciosMultiples(char *texto);//I
int verifEspacios(char *texto);// ----------------------

/*---------- INSTRUCCIONES --------------------------------*/
Registros* crearRegistro();//G
int ejecutarPrograma(PCB *ejec);//F -----------------
int obtenerRegistro(char nombre[], Registros *registros, int **registro);//J
int obtenerValor(Token *tok);//I
int mov(Token *param);//G
int add(Token *param);//F -----------------
int sub(Token *param);//F  --------------
int mul(Token *param);//F -------------
int divR(Token *param);//G
int inc(Token *param);//I
int dec(Token *param);//G

/*---------- VENTANAS --------------------------------*/
int kbhit(void);//F  ----------------
WINDOW *crearVentana(int altura, int anchura, int posY, char *nombre);//J
void impVentanaComandos(WINDOW *ventana);//I
void limpiarComando(WINDOW *ventana);//G
void impEncabezado(WINDOW *ventana);//F ------------------
void impInstruccVentana(WINDOW *ventana, PCB *PCB, int resultado);//J
void limpiarVentana(WINDOW *ventana, char *nomVentana);//I

/*---------- COMANDOS --------------------------------*/
int detectarComando(char cad[]);//G
char *sacarNomArchivo(char cad[]);//F ------------------
void leerComando(WINDOW *ventana, int *pos, char cad[], int caracter);//J
void borrarCaracter(WINDOW *ventana, int *pos);//I
void imprimirCaracter(WINDOW *ventana, int *pos, char cad[], int caracter);//G

/* --------- LOG ERRORES ----------------------------*/
void detectarError(WINDOW *ventana, int error);//F 
const char *mensajeEstatus(int codigo);//J

/*------------------------- Posibles funciones ----------------------------------------------*/
// Estas funciones son como las posibles en las que nos podemos guiar, de preferencia mantenerlas
// pero si tienen que modificar el tipo de retorno o los parámetros está bien, son sólo como 
// sugerencias de como podrían ser las funciones en las que vamos a trabajar
/* COLAS Y CONTEXTO */
Cola* crearCola(void);
void  encolar(Cola *c, PCB *p);        /* deja p->siguiente = NULL */
PCB*  extraer(Cola *c);                /* NULL si está vacía */
int   colaVacia(Cola *c);
void  liberarCola(Cola *c);
void  cargarContexto(PCB *p);          /* *reg  <- p->registro */
void  guardarContexto(PCB *p);         /* p->registro <- *reg */

/* CARGA DE ARCHIVOS */
int   cargarArchivo(char *nombre, Cola *listos);   /* BIEN o error; si falla, no encola y libera lo creado */
void  mostrarErrorArchivo(WINDOW *cmd, char *nombre, int error);

/* COMANDOS */
int   separarArchivos(char cad[], char *nombres[], int max);  /* regresa cuántos nombres */

/* VENTANAS */
void  impEjecucion(WINDOW *v, PCB *p, int resultado);
void  impListos(WINDOW *v, Cola *c);        /* IR y Status en "-" */
void  impTerminados(WINDOW *v, Cola *c);    /* últimos valores y última instrucción */ 

#endif
