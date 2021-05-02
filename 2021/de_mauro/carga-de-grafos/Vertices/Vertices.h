#ifndef VERTICES_H
#define VERTICES_H

#include "../Tipos/Tipos.h"
#include <string.h>

struct vecino_t {
    orden_natural_t orden_natural;
    peso_t peso;
};

typedef struct vecino_t vecino_t;


struct Vertice {
    nombre_t nombre;
    color_t color;
    vecino_t *vecinos;
    u32 cantidad_de_vecinos;
};

typedef struct Vertice Vertice;

struct Vertices {
    Vertice *vertices;
    u32 numero_de_vertices;
};

typedef struct Vertices Vertices;

#endif
