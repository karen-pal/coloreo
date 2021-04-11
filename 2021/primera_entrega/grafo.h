#include "GrafoSt21.h"
#include <stdbool.h>
int existe_nodo (Grafo grafo,int i, u32 first_node_name);
void inicializar_nodo(Grafo grafo, int array_index, int nodo1, int nodo2);
void agregar_vecino(Grafo grafo, int index, int nodo);
bool GraphParse(Grafo grafo, FILE *stream);
int cmpfunc (const void * a, const void * b);
