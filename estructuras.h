#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

#define NUM_RECURSOS     3      /* recursos X, Y, Z */
#define LIMITE_RECURSOS  8      /* total del sistema por recurso */

typedef struct Token{
    char *textoToken;
    int tipoParam;
    struct Token *sig;
} Token;

typedef struct Renglon{
    char *texto;            /* Texto original del renglon, sin salto de linea */
    int error;              /* BIEN o el error que se detecto al cargar el archivo */
    struct Renglon *sig;
    Token *primerToken;
    Token *ultimoToken;
} Renglon;

typedef struct {
    Renglon *inicio;
    Renglon *final;
    int tamanio;
} Archivo;

/*
 * Banco de registros activo de la CPU.
 * La variable global `reg` apunta al contexto que se está ejecutando.
 */
typedef struct {
    int EAX;    int EBX;
    int ECX;    int EDX;
} Registros;

extern Registros *reg;

extern int Q;                           /* quantum: instrucciones por turno (main.c) */
extern int recursos[NUM_RECURSOS];      /* recursos disponibles del sistema (main.c) */

/*
 * Cada PCB conserva una copia propia de sus registros.
 * `siguiente` enlaza el PCB con el siguiente elemento de una cola FIFO.
 */
typedef struct PCB{
    Registros *registro;
    char *nombre;           /* Nombre del proceso */
    Archivo *programa;
    int PC;
    Renglon *IR;
    int estado;
    int espera;
    int id;
    int max[NUM_RECURSOS];      /* MAX x,y,z: maximo que declara el proceso */
    int usado[NUM_RECURSOS];    /* recursos X, Y, Z que tiene asignados ahora */
    int ejecutadas;             /* instrucciones ejecutadas en el quantum actual */
    int estatusFinal;           /* BIEN, KILLED o el error que lo termino */
    struct PCB *siguiente;
} PCB;

/*
 * Cola FIFO de PCB. Los propios PCB forman los nodos mediante `siguiente`;
 * por ello, un mismo PCB no puede estar simultaneamente en dos colas.
 */
typedef struct {
    PCB *frente;
    PCB *final;
    int tamanio;
} Cola;

/*------------ TIPO DE PARAMETRO  ---------------------------------*/
typedef enum tipoParametro{
    INSTR = 1, REG_COM, REG, NUM, NUM_COM   /* NUM_COM: numero con coma, ej. "1," */
} tipoParametro;

/*------------ COMANDO EN LA TERMINAL  ---------------------------------*/
typedef enum comandoTerminal{
    SALIR = 10, EJECUTAR_ARCHIVO, KILL_PROCESO
} comandoTerminal;

/*------------ ESTADOS DEL SISTEMA  ---------------------------------*/
typedef enum estadoSistema{
    EJECUCION = 50, ESPERA, TERMINADO, NUEVO, BLOQUEADO
} estadoSistema;

/*------------ ERRORES Y ADVERTENCIAS ---------------------------------*/
typedef enum errorAdvertencia{
    BIEN = 0,
/*----- Archivos -----*/
    NOMBRE_INCORRECTO = 100,
    NO_HAY_ARCHIVO,
    NO_HAY_TEXTO,

/*----- Tokens -----*/
    LINEA_VACIA = 200,

/*----- Lexico -----*/
    TIPO_PARAM_INVALIDO = 300,
    INICIA_MINUSCULA,
    REGISTRO_INVALIDO,
    NUM_DECIMAL,

/*----- Sintactico -----*/
    INSTR_REG_INCUMPLIDA = 400,
    PARAMETROS_INCORRECTOS,
    PARAMETROS_INSUFICIENTES,
    PARAMETROS_EXTRA,
    DIV_ENTRE_CERO,
    ESPACIOS_EXTRA,
    INSTRUCCION_NO_RECONOCIDA,
    COMA_CON_ESPACIOS,

/*----- Comandos -----*/
    COMANDO_INVALIDO = 500,

/*----- Recursos -----*/
    FALTA_MAX = 600,            /* la primera instruccion no es MAX */
    MAX_EXCEDE_LIMITE,          /* algun valor de MAX es mayor que LIMITE_RECURSOS */
    GET_EXCEDE_LIMITE,          /* usado + pedido supera LIMITE_RECURSOS */
    NUMEROS_INVALIDOS,          /* MAX/GET con valores no validos (negativos, etc.) */

/*----- Procesos -----*/
    KILLED = 700,
    PROCESO_NO_EXISTE
} errorAdvertencia;

#endif
