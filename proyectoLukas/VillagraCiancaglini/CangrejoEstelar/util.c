#include "veinteveinte.h"

u32 NumeroDeVertices(Grafo G) { return G->V->length; }

u32 NumeroDeLados(Grafo G) { return G->nsides; }

u32 Delta(Grafo G) { return G->delta; }
