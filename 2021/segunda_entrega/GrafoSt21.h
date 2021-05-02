#ifndef __gs_H
#define __gs_H
#include <stdint.h>

typedef uint32_t u32;

typedef struct {
    u32 vecino; //nombre
    u32 peso;
}LadoConPeso;

typedef struct {
    LadoConPeso *vecinos;
    u32 nombre;
    u32 grado;
    u32 color;
}NodoSt;

typedef struct {
    u32 cant_ver;
    u32 cant_lad;
    NodoSt *nodos_array;
    u32 *orden; //posiciones
}GrafoSt;

#endif
