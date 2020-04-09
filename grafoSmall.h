
#ifndef __gs_H
#define __gs_H
#include <stdint.h>
typedef struct {
    uint32_t vecinos[100]; //solo guardamos los nombres
    uint32_t nombre;
    uint32_t grado;
    uint32_t color;
}NodoSt;

typedef struct {
    uint32_t cant_ver;
    uint32_t cant_lad;
    NodoSt nodos_array[100]; // y si aca esta el orden?
    uint32_t orden[100]; // esto no hace falta
}GrafoSt;

#endif
