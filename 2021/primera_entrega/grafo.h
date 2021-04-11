
#ifndef __g_H
#define __g_H
#include "GrafoSt21.h"
#include <stdbool.h>
typedef struct{
	u32* nodos_1;
	u32* nodos_2;
	bool result;
} Result;

typedef struct{
	u32 nodo1;
	u32 nodo2;
} Tupla;

int existe_nodo (Grafo grafo,int i, u32 first_node_name);
void inicializar_nodo(Grafo grafo, int array_index, int nodo1, int nodo2);
void agregar_vecino(Grafo grafo, int index, int nodo);
Result GraphParse(Grafo grafo, FILE *stream,u32* temp_nodos_1,u32* temp_nodos_2);
int cmpfunc (const void * a, const void * b);


#endif
