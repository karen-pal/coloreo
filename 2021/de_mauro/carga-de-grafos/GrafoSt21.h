#ifndef GRAFO_ST
#define GRAFO_ST

#include "Ordenamiento/Ordenamiento.h"
#include "Vertices/Vertices.h"

typedef
    struct GrafoSt {
        u32 numero_de_lados;
        u32 delta;

        Ordenamiento ordenamiento;
        Vertices vertices;
    }
    GrafoSt;

typedef unsigned int u32;

#endif
