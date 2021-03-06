#ifndef __gs_H
#define __gs_H
#include <stdint.h>

typedef uint32_t u32;

typedef struct {
    u32 *vecinos; //solo guardamos los nombres
    u32 nombre;
    u32 grado;
    u32 color;
}NodoSt;

typedef struct {
    u32 cant_ver;
    u32 cant_lad;
    NodoSt *nodos_array;
    u32 *orden;
    u32 *array_nat;
}GrafoSt;



#endif
