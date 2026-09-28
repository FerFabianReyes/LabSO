# Tareas pendientes para integrar las ideas de E2

Cada tarea se hace en su propia rama. Al terminar: `make` sin warnings,
probar con `a.asm` y `errores.asm` y comparar que los resultados sean los mismos.

## Tarea 1: `obtenerValor` (instrucciones.c)

`mov`, `add`, `sub`, `mul` y `divR` repiten el mismo bloque:

```c
if (param->sig->sig->tipoParam == REG) {
    int *regValor = obtenerRegistro(param->sig->sig->textoToken);
    ... *regValor ...
} else {
    int num = atoi(param->sig->sig->textoToken);
    ... num ...
}
```

1. Crear `int obtenerValor(Token *tok)` que regrese el valor del token:
   el contenido del registro si `tok->tipoParam == REG`, o el numero
   convertido con `atoi` si es `NUM`. (Es la misma idea de `obtenerValor`
   en el codigo de E2, pero trabajando con tokens.)
2. Agregar su prototipo en `prototipos.h`.
3. Usarla en las cinco funciones para quitar el `if/else` repetido.
4. Opcional: juntar las siete funciones dentro de `ejecutarPrograma`.

Cuidado: `divR` debe seguir regresando `DIV_ENTRE_CERO` si el divisor es 0.

## Tarea 2: `esEntero` (lexicoSintactico.c)

`esNumero` usa expresiones regulares para saber si un token es entero.
Reemplazar esa parte por una funcion `int esEntero(char texto[])` que
recorra la cadena con `isdigit`, como la de E2, pero que acepte tambien
el signo `+` ademas del `-`.

Los decimales (`2.5`) deben seguir regresando `NUM_DECIMAL`.

## Tarea 3: Archivos de prueba

Crear una carpeta `pruebas/` con un archivo .asm por cada error del enum
en `estructuras.h` y anotar en este documento que Status y que mensaje de
la ventana de errores debe mostrar cada uno.
