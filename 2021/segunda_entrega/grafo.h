#ifndef __g_H
#define __g_H

#include "GrafoSt21.h"
#include <stdbool.h>

typedef struct{
	u32 nodo1;
	u32 nodo2;
} Tupla;

typedef struct{
	Tupla * array_nodos;
	bool result;
} Result;



Result ParsearGrafo(Grafo grafo, FILE *stream, Tupla * array_nodos);
int cmpfunc (const void * a, const void * b);
u32* ContarGrados(Tupla * array_nodos, u32 vert, u32 lados);


#endif
