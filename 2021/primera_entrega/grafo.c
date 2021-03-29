
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "GrafoSt2021.h"
#include "veinteveinte.h"

enum LineStart {
    LineComment = 'c',
    LineGraphConfig = 'p',
    LineGraphEdge = 'e',
    LineNewLine = '\n',
    LineCarriageReturn = '\r'
};

int existe_nodo (Grafo grafo,int i, int first_node_name) {
    int res = -1;
    for (int j=0; j<i; j++) {
        if (grafo->nodos_array[j].nombre == -1){
            break;
        } else if (grafo->nodos_array[j].nombre == first_node_name) {
            res = j;
            break;
        }
    }
    return res;
}

void inicializar_nodo(Grafo grafo, int array_index, int nodo1, int nodo2) {
    grafo->nodos_array[array_index].nombre = nodo1;
    grafo->nodos_array[array_index].grado = 1;
    grafo->nodos_array[array_index].vecinos = malloc(sizeof(u32));
    grafo->nodos_array[array_index].vecinos[0] = nodo2;
    grafo->orden[array_index] = array_index;
}

void agregar_vecino(Grafo grafo, int index, int nodo){
    grafo->nodos_array[index].grado++;
    grafo->nodos_array[index].vecinos = realloc(grafo->nodos_array[index].vecinos,
                                            grafo->nodos_array[index].grado * sizeof(u32));
    grafo->nodos_array[index].vecinos[grafo->nodos_array[index].grado-1] = nodo;
}


bool GraphParse(Grafo grafo, FILE *stream) {

    char line;
    u32 nodes = 0;
    u32 edges = 4294967295;
    u32 node1, node2 = 0;
    int readchars = 0;
    u32 curredge = 0;
    int array_index = 0;

    // Error handling
    int matched_params = 0;
    while ((readchars = fscanf(stream, "%c", &line)) != 0) {
        if (readchars == EOF || curredge >= edges) {
            break;
        }

        switch (line) {
        case LineComment:
            printf("%s: found a comment, skipping...", __func__);

            // A bit to unwind here, but simplifies things a lot,
            // as it helps us completely skip comments:
            //   '*' means ignore this value, don't try to assign it
            //   '[]' matches a regular expression within
            //   '^\n' means everything from the beginning ('^') til
            //   the newline ('\n')
            fscanf(stream, "%*[^\n]\n");

            break;

        case LineGraphConfig:
            // Ignores the 'edge' word and only assigns the other values
            matched_params = fscanf(stream, "%*4s%u%u", &nodes, &edges);
            if (matched_params != 2) {
                printf("couldn't parse config: nodes='%u', edges='%u'",nodes,edges);
                return false;
            }

            printf("found graph config: nodes=%u, edges=%u",
                      nodes, edges);
            grafo->cant_ver = nodes;
            grafo->cant_lad = edges;
            grafo->nodos_array = malloc(grafo->cant_ver * sizeof(NodoSt));
            grafo->orden = malloc(grafo->cant_ver * sizeof(u32));
            /*inicializacion del array*/
            for (u32 index = 0; index < grafo->cant_ver; index++) {
                grafo->nodos_array[index].nombre= -1;
                grafo->nodos_array[index].vecinos= NULL;
                grafo->nodos_array[index].grado= 0;
                grafo->nodos_array[index].color= 4294967295;//2^32-1;
            }
            break;

        case LineGraphEdge:
            matched_params = fscanf(stream, "%u%u", &node1, &node2);
            if (matched_params != 2) {
                printf("couldn't parse edge: (%u, %u)", node1,
                          node2);
                return false;
            }

            int indice_nodo1 = existe_nodo(grafo,array_index,node1);
            int indice_nodo2 = existe_nodo(grafo,array_index, node2);
            printf("HOLA SOY PRUEBA %d\n", indice_nodo1);
            printf("HOLA SOY PRUEBA2 %d\n", indice_nodo2);
            printf("HOLA SOY INDEX %d\n", array_index);
            if (indice_nodo1 == -1 && indice_nodo2 == -1) {
                inicializar_nodo(grafo,array_index,node1,node2);
                array_index++;
                inicializar_nodo(grafo,array_index,node2,node1);
                array_index++;
            } else if (indice_nodo1 == -1 && indice_nodo2 != -1) {
                inicializar_nodo(grafo,array_index,node1,node2);
                agregar_vecino(grafo, indice_nodo2, node1);
                array_index++;
            } else if (indice_nodo1 != -1 && indice_nodo2 == -1) {
                inicializar_nodo(grafo,array_index,node2,node1);
                agregar_vecino(grafo, indice_nodo1, node2);
                array_index++;
            } else {
                agregar_vecino(grafo, indice_nodo2, node1);
                agregar_vecino(grafo, indice_nodo1, node2);
            }
        

            break;

        case LineNewLine:
        case LineCarriageReturn:  // some graphs come from Windows
            break;

        default:
            printf("%d",668);
            printf("couldn't parse graph: found '%c'", line);
            return false;
        printf("%d",667);
        }
    }
}

int cmpfunc (const void * a, const void * b) {
   return ( *(int*)a - *(int*)b );
}

Grafo ConstruccionDelGrafo(void) {
    Grafo grafo = malloc(sizeof(GrafoSt));
    printf("%d",110);
    bool ok = GraphParse(grafo, stdin);
   /* if (!ok) {
        GraphDestroy(g);
        return NULL;
    }*/

    grafo->array_nat = malloc(sizeof(u32)*NumeroDeVertices(grafo));
    u32 *array_natural = malloc(sizeof(u32)*NumeroDeVertices(grafo));
    for (u32 indice = 0; indice < NumeroDeVertices(grafo); indice++) {
            array_natural[indice] = grafo->nodos_array[indice].nombre;

        }
    qsort(array_natural, NumeroDeVertices(grafo), sizeof(u32), cmpfunc);
    for (u32 i = 0; i < NumeroDeVertices(grafo); i++) {
        for (u32 j=0; j<NumeroDeVertices(grafo);j++){
            if (grafo->nodos_array[j].nombre == array_natural[i]){
                grafo->array_nat[i] = j;
                break;
            }
        }
    }
    /*if (colors == U32_MAX) {
        GraphDestroy(g);
        return NULL;
    }*/

    return grafo;
}



int main (void) {
    Grafo grafo = ConstruccionDelGrafo();
    for (int index = 0; index<NumeroDeVertices(grafo); index++) {
        printf("NOMBRE: %u \nCOLOR: %u \nGRADO: %u \n", 
                Nombre(index,grafo), Color(index,grafo),Grado(index,grafo));
        for (int vecindex= 0; vecindex < Grado(index,grafo);vecindex++){
            printf("Vecino %u: %u\n", vecindex,NombreVecino(vecindex, index, grafo));
        }
        printf("\n");
    }
    return 0;
}

