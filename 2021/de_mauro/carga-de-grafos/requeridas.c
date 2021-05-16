#include "requeridas.h"
#include "RomaVictor.h"
#include "GrafoSt21.h"
#include <stdlib.h>
#include <stdint.h>

#define U32_MAX UINT32_MAX

#define no_color (UINT32_MAX - 1)


u32 MaxColor(Grafo G) {
    u32 max_color = 0 ;
    for (u32 indice = 0; indice < NumeroDeVertices(G); indice++) {
        if (Color(indice, G) > max_color) {
            max_color = Color(indice, G);
        }
    }
    return max_color;
}

void Bfs (u32 x, u32 *vertices_cc, Grafo G) {
    if (vertices_cc[x] == 0) {
        vertices_cc[x] = 1;
    }
    for (u32 j = 0; j < Grado(x,G); j++) {
        int indice = OrdenVecino(j,x,G);
        if (vertices_cc[indice] == 0) {
            vertices_cc[indice] = 2;
        }
    }
    for (u32 j = 0; j < Grado(x,G); j++) {
        u32 indice = OrdenVecino(j,x,G);
        for (u32 i = 0; i < Grado(indice,G); i++) {
            if (vertices_cc[OrdenVecino(i,indice,G)] == 0) {
                Bfs(OrdenVecino(i,indice,G), vertices_cc, G);
            }
        }
    }
}


void arreglo_colores_vecino(u32 vertice, Grafo G, u32 * color_vecinos, u32 grado){
    for (u32 i = 0; i < grado; i++) {
            color_vecinos[i] = ColorVecino(i, vertice,G);
    }
}
int _natural_compare(const void *_a, const void *_b) {
    u32 a = *(u32 *)_a;
    u32 b = *(u32 *)_b;
    if (a == b)
        return 0;
    else if (a < b)
        return -1;
    else
        return 1;
}

//[0,3,4]
//[0,101,102]
//[0,1,1,20000000, 2000000]
//[0,20]
u32 encontrar_minimo_color(u32 i , u32 * color_vecinos, u32 grado, Grafo grafo){
    u32 color = 0;
    //if (i == 8){
    //	printf(">>>>> colores de vecinos de vertice ANTES ORD %u\n\n", i);
    //	for (int k=0; k<grado;k++){
    //		printf("%u : %u\n",NombreVecino(k,i,grafo) ,color_vecinos[k]);
    //	}
    //	printf("\n\n");
    //}
    qsort(color_vecinos,grado, sizeof(u32),_natural_compare);
    //if (i == 8){
    //	printf(">>>>> colores de vecinos de vertice DESP ORD %u\n\n", i);
    //	for (int k=0; k<grado;k++){
    //		printf("%u : %u\n",NombreVecino(k,i,grafo) ,color_vecinos[k]);
    //	}
    //	printf("\n\n");
    //}
    if (color_vecinos[0]>0) return color;
    for (u32 j=0; j<grado; j++){
       // printf("color actual %u, color + 1  %u\n", color_vecinos[j], color + 1);
        if (color_vecinos[j] == color + 1) {
           // printf("entre al primer if");
            color++;
        } else  if (color_vecinos[j] != color) {
           // printf("entre al segundo\n");
            break;
        }
    }
    color++;
   // printf("return %u", color);
    return color;

}

u32 Greedy(Grafo G) {
    u32 max_color = 0;
    for (u32 j = 0; j < NumeroDeVertices(G); j++) {
        FijarColor(-1, j,G);
	//printf("vert %u: color %u\n", Nombre(j,G), Color(j,G));
    }
    FijarColor(0,0,G);
    for (u32 i = 1; i < NumeroDeVertices(G); i++) {
    	u32 * color_vecinos = malloc(Grado(i,G)* sizeof(u32));
	    arreglo_colores_vecino(i,G, color_vecinos, Grado(i,G));
	    u32 min_color = encontrar_minimo_color(i,color_vecinos, Grado(i,G),G);

        if (min_color > max_color) {
            max_color = min_color;
        }
        FijarColor(min_color, i, G);
	free(color_vecinos);
    }
    return max_color + 1;
}


char Bipartito(Grafo G) {
    u32 *vertices_cc = calloc(NumeroDeVertices(G),sizeof(u32));
    if (vertices_cc == NULL){
        return 0; //ver
    }
    for(u32 i = 0; i < NumeroDeVertices(G); i++) {
        if (vertices_cc[i] == 0) {
            Bfs(i, vertices_cc, G);
        }
    }
    for(u32 i = 0; i < NumeroDeVertices(G); i++) {
        FijarColor(vertices_cc[i]%2,i,G);
    }
    int color = 2;
    for(u32 i = 0; i < NumeroDeVertices(G); i++) {
        for(u32 j = 0; j < Grado(i,G); j++) {
            if (Color(i,G) == ColorVecino(j, i, G)) {
                FijarColor(color,OrdenVecino(j, i, G),G);
                color++;
            }
        }
    }
    free(vertices_cc);
    if (MaxColor(G) == 1) {
        return 1;
    } else {
        return 0;
    }
}



char AleatorizarVertices(Grafo G, u32 R){
    srand(R);
    u32 N = NumeroDeVertices(G);
    u32 *array_random = calloc(N,sizeof(u32));
    if (array_random == NULL){
        return 1;
    }
    u32 i = 0;
    for (int k=0; k<N;k++){
	array_random[k]=k;
    }
    for (u32 i = 0; i < N * 10; i++) {
        u32 p1 = rand() % N;
        u32 p2 = rand() % N;
        u32 v1 = array_random[p1];
        u32 v2 = array_random[p2];
        array_random[p1] = v2;
        array_random[p2] = v1;
    }
    for (u32 i = 0; i < N; i++) {
        char res = FijarOrden(i, G, array_random[i]);
        if (res != 0) {
    	    free(array_random);
            return 1;
        }
    }

    free(array_random);
    array_random=NULL;
    return 0;
}

u32* copiar_arreglo(void *orig, size_t element_size, u32 length) {
    u32 capacity = max(8, length);

    void *data = calloc(capacity, element_size);

    memcpy(data, orig, length * element_size);

    return data;
}


int _mayor_menor_comp(const void *_a, const void *_b) {
    u32 a = *(u32 *)_a;
    u32 b = *(u32 *)_b;
    if (a == b)
        return 0;
    else if (a > b)
        return -1;
    else
        return 1;
}
bool check_permutation ( u32* perm, u32 color){
    u32* perm_ord = copiar_arreglo(perm, sizeof(u32), color);
    qsort(perm_ord, color, sizeof(u32), _natural_compare);
    for (int i=0; i<color; i++){
        if (perm_ord[i] != i){
	    printf("elm %d no es %d\n", perm_ord[i],i);
	    free(perm_ord);
            return false;
        }
    }
    free(perm_ord);
    return true;
}
u32 * calcular_natural_array(Grafo G){
    u32 N = NumeroDeVertices(G)
    u32 * result[N];
    u32 * vertices[N];
    for (int i=0; i< N; i++){
        vertices[i] =struct(Nombre(i,G), i) //seria ideal
    }
    //order_by_Nombre me tiene que dar los i
    order_by_Nombre(vertices);

    //i = 2, Nombre(i,G) = 9
    //ordenar y queda
    //[(9,2), ...] //vertices
    //implica que el vertice 2 del orden interno està en la posicion 0 del ord natural
    //
    //
    //idea de order_by_nombre:
    order_by_Nombre(vertices):
        qsort;
        for (int index=0; index<N;i++){
        	result[vertices[index].i] = index;
        }
    return result;
}
char OrdenPorBloqueDeColores(Grafo G, u32 * perm){
    u32 X = Greedy(G);

    printf("color %u \n", X);
    bool is_perm = check_permutation(perm, X);
    printf("is perm? %s\n",is_perm?"true":"false");
    u32 color;
    Result result;
    //Grafo copia = CopiarGrafo(G);
    // X = 4
    // perm = 3,2,1,0
    int offset=0
    // natural_array[0] es la posicion en el arreglo natural donde iria el vértice 0 del orden_interno
    u32 * natural_array = calcular_natural_array(G);
    for (int i=0; i<X; i++){
        color=perm[i];
	// result = u32 * nodos , int cantidad
	// indices del orden natural
	result = nodosDeColor(color, G);
	for (int j=offset; j<offset+result.cantidad; j+++){
		// fijarOrden(dest,G,src)
		fijarOrden(j,G,natural_array[result.nodos[j]]);
	}
	offset = offset+result.cantidad;
    }
    return 0;
}
//0  1 2 3
//[0,1,2,3]
//perm = 3,2,1,0
//orden_interno = [2,3,1,0]
//
//fijarOrden(0,copia,3)
//G = [3,3,1,0]
