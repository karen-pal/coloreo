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
int _struct_compare(const void *_a, const void *_b) {
    Result a = *(Result *)_a;
    Result b = *(Result *)_b;
    if (a.nombre_nodo == b.nombre_nodo)
        return 0;
    else if (a.nombre_nodo < b.nombre_nodo)
        return -1;
    else
        return 1;
}
u32 * calcular_natural_array(Grafo G){
    printf("Empiezo nat array\n");
    u32 N = NumeroDeVertices(G);
    printf("Este grafo tiene %u vertices\n", N);
    u32 result[N];
    Result vertices[N];
    for (int i=0; i< N; i++) {
        vertices[i].nombre_nodo = Nombre(i,G);
        //printf("metiendo nombre %u, de pos %u\n",Nombre(i,G),i);
        vertices[i].indice_interno = i;
    }
    //ordenamos por Nombre me tiene que dar los i
    qsort(vertices, N, sizeof(Result), _struct_compare);
    //printf("Imprimiendo qsort...\n");
    //for (int j=0; j<N; j++){
    //    printf("%d: %u con pos interna %u\n", j, vertices[j].nombre_nodo, vertices[j].indice_interno);
    //}
    for (int index=0; index<N;index++) {
        printf("el vert de pos int %u esta en la pos nat %d \n", vertices[index].indice_interno, index);
        result[vertices[index].indice_interno] = index; //(index, color)
    }
    printf("Imprimiendo array natural...\n");
    for (int j=0; j<N; j++){
        printf("%d= %u\n", j, result[j]);
    }
    //i = 2, Nombre(i,G) = 9
    //ordenar y queda
    //[(9,2), ...] //vertices
    //implica que el vertice 2 del orden interno està en la posicion 0 del ord natural
    //si quiero ver en que pos nat está el vértice con posicion 5 en el orden interno
    //entonces eso me lo da hacer
    //result[5]
    return result;
}

// [{posicion: posicion_natural1, key: color}]
//fijarOrden(0, G, posicion_natural0...)

void countingSort(int array[], int size) {
  int output[10];

  // Find the largest element of the array
  int max = array[0];
  for (int i = 1; i < size; i++) {
    if (array[i] > max)
      max = array[i];
  }

  // The size of count must be at least (max+1) but
  // we cannot declare it as int count(max+1) in C as
  // it does not support dynamic memory allocation.
  // So, its size is provided statically.
  int count[10];

  // Initialize count array with all zeros.
  for (int i = 0; i <= max; ++i) {
    count[i] = 0;
  }

  // Store the count of each element
  for (int i = 0; i < size; i++) {
    count[array[i]]++;
  }

  // Store the cummulative count of each array
  for (int i = 1; i <= max; i++) {
    count[i] += count[i - 1];
  }

  // Find the index of each element of the original array in count array, and
  // place the elements in output array
  for (int i = size - 1; i >= 0; i--) {
    output[count[array[i]] - 1] = array[i];
    count[array[i]]--;
  }

  // Copy the sorted elements into original array
  for (int i = 0; i < size; i++) {
    array[i] = output[i];
  }
}



char OrdenPorBloqueDeColores(Grafo G, u32 * perm){
    u32 X = Greedy(G);

    printf("color %u \n", X);
    bool is_perm = check_permutation(perm, X);
    printf("is perm? %s\n",is_perm?"true":"false");
    if (!is_perm){
        return 0;
    }
    //Grafo copia = CopiarGrafo(G);
    //X = 4
    //perm = 3,2,1,0
    int offset=0;
    //natural_array[0] es la posicion en el arreglo natural donde iria el vértice 0 del orden_interno
    u32 * natural_array = calcular_natural_array(G);
    //countingSortByColor(natural_array, X,);
    //for (int i=0; i<X; i++){
    //    color = perm[i];
    //    //result = u32 * nodos , int cantidad
    //    //indices del orden natural
    //    for (int j=offset; j<offset+result.cantidad; j+++){
    //        //fijarOrden(dest,G,src)
    //        fijarOrden(j,G,natural_array[result.nodos[j]]);
    //    }
    //    offset = offset+result.cantidad;
    //}
    return 1;
}
//0  1 2 3
//[0,1,2,3]
//perm = 3,2,1,0
//orden_interno = [2,3,1,0]
//
//fijarOrden(0,copia,3)
//G = [3,3,1,0]
