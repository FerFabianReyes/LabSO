#ifndef ESTRUCTURAS_H
#define ESTRUCTURAS_H

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

typedef struct {
    int EAX;    int EBX;
    int ECX;    int EDX;
} Registros;

extern Registros *reg;

typedef struct {
    char *nombre;           /* Nombre del proceso */
    Archivo *programa;
    int PC;
    Renglon *IR;
    int estado;
    int espera;
    int id;
} Ejecucion;

/*------------ TIPO DE PARAMETRO  ---------------------------------*/
typedef enum tipoParametro{
    INSTR = 1, REG_COM, REG, NUM
} tipoParametro;

/*------------ COMANDO EN LA TERMINAL  ---------------------------------*/
typedef enum comandoTerminal{
    SALIR = 10, EJECUTAR_ARCHIVO
} comandoTerminal;

/*------------ ESTADOS DEL SISTEMA  ---------------------------------*/
typedef enum estadoSistema{
    EJECUCION = 50, ESPERA
} estadoSistema;

/*------------ ERRORES Y ADVERTENCIAS ---------------------------------*/
typedef enum errorAdvertencia{
    BIEN = 0,
/*----- Archivos -----*/
    NOMBRE_INCORRECTO = 100,
    NO_HAY_ARCHIVO,
    NO_HAY_TEXTO,

/*----- Tokens -----*/
    NO_HAY_TOKENS = 200,
    LINEA_VACIA,

/*----- Lexico -----*/
    INICIA_EN_NUMERO = 300,
    TIPO_PARAM_INVALIDO,
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
    COMANDO_INVALIDO = 500
} errorAdvertencia;

#endif
