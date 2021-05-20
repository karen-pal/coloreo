#include "requeridas.h"
#include "RomaVictor.h"
#include "GrafoSt21.h"
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>
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
void calcular_natural_array(Grafo G, u32 * result){
    printf("Empiezo nat array\n");
    u32 N = NumeroDeVertices(G);
    //printf("Este grafo tiene %u vertices\n", N);
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
        //printf("el vert de pos int %u esta en la pos nat %d \n", vertices[index].indice_interno, index);
        result[vertices[index].indice_interno] = index; //(index, color)
    }
    //printf("Imprimiendo array natural...\n");
    //for (int j=0; j<N; j++){
    //    printf("%d= %u con nombre %u de color %u\n", j, result[j], Nombre(j,G),Color(j,G));
    //}
    //i = 2, Nombre(i,G) = 9
    //ordenar y queda
    //[(9,2), ...] //vertices
    //implica que el vertice 2 del orden interno està en la posicion 0 del ord natural
    //si quiero ver en qué pos nat está el vértice con posicion 5 en el orden interno
    //entonces eso me lo da hacer
    //result[5]
    //result te hace el mapping ord_interno->orden_natural
}

// [{posicion: posicion_natural1, key: color}]
//fijarOrden(0, G, posicion_natural0...)

//void countingSortByColor(u32 * array, int size, u32 max, Grafo G) {
//  // en vez de esto: int output[size];
//  // vamos a hacer FijarOrden
//
//  // Find the largest element of the array
//  u32 output[size];
//  u32 count[max+1];
//  Count count[max+1]
//  inicializar_count:
//    for (int i=0; i< max+1; i++){
//        count[i].indices = calloc(N,sizeof(u32));
//    }
//  // Initialize count array with all zeros.
//  for (int i = 0; i <= max; ++i) {
//    count[i] = 0;
//  }
//
//  // Store the count of each element
//  printf("size: %u vs max+1=%u \n",size, max+1);
//  for (u32 i = 0; i < size; i++) {
//    //printf("considerando i: %u con color:%u\n",i, Color(i,G));
//    count[Color(i,G)].count++; //orden interno
//    count[Color(i,G)].indices[count[Color(i,G)].count] = i;
//    //printf("tiene %u elems\n",i,Color(i,G), count[Color(i,G)]);
//  }
//  //[(count, [nombres])]
//  //reorder_by_indexes(count, perm);
//  // Store the cummulative count of each array
//  for (u32 i = 1; i <= max; i++) {
//    count[i] += count[i - 1];
//    //printf("count[%u]=%u\n",i, count[i]);
//  }
//  // Find the index of each element of the original array in count array, and
//  // place the elements in output array
//  for (int i = size - 1; i >= 0; i--) {
//    // counts[ interno->natural ]
//    output[count[i] - 1] = array[i];
//    //FijarOrden(count[Color(i,G)] - 1,G,array[i] );
//    count[Color(i,G)]--;
//  }
//  // Copy the sorted elements into original array
//  for (int i = 0; i < size; i++) {
//    FijarOrden(i,G,output[i]);
//    //count[Color(i,G)] - 1,G,array[i] );
//    //array[i] = output[i];
//  }
//
//}

void buildArray(Grafo G, Elem * index_and_color, u32 * color_counts){
    for (int i=0; i<NumeroDeVertices(G);i++){
        index_and_color[i].indice_orig = i;
        index_and_color[i].color = Color(i,G);
        color_counts[Color(i,G)]++;
        //printf("para el color %u hay %u counts\n",Color(i,G),color_counts[Color(i,G)]);
    }
}

int build_chunks(u32 color,Elem * index_and_color,u32 N,u32** chunks,int i){
    int amount = 0;
    //for (int k = 0; k<3; k++){
    //    printf("en chunks[%d]=%u\n",k,chunks[k]);
    //}
    //printf(">>>Considerando color %u\n",color);
    for (int j=0; j<N; j++){
        if (index_and_color[j].color == color) {
            //printf("escribiendo en chunks[%d][%d] = %u\n",i,amount,index_and_color[j].indice_orig);
            chunks[i][amount] =index_and_color[j].indice_orig;
            amount++;
        }
    }
    return amount;
}


char OrdenPorBloqueDeColores2(Grafo G, u32 * perm){
    //Greedy(G); //pero esto cambia el color????? no hay que hacerlo?
                        //sin hacer esto como sabés r?

    u32 N = NumeroDeVertices(G);
    u32 natural_array[N];
    calcular_natural_array(G, natural_array);
    for (int i= 0; i< N; i++){
        FijarOrden(i,G,natural_array[i]);
    }
    u32 X = MaxColor(G)+1;
    printf("despues de orden natural\n");
    for (u32 i=0; i<NumeroDeVertices(G);i++){
        printf("Vertice  %u ---- Color: %u\n", Nombre(i,G), Color(i,G));
    }
    bool is_perm = check_permutation(perm, X);
    printf("is perm? %s\n",is_perm?"true":"false");
    if (!is_perm){
        return 0;
    }
    Elem index_and_color[N];
    u32 color_counts[X];
    for (int i=0; i<X;i++){
        color_counts[i]=0;
    }
    buildArray(G, index_and_color, color_counts);

    //for (int m=0; m<X; m++){
    //    printf("color_counts[%d]=%u\n",m,color_counts[m]);
    //}
    //for (int m=0; m<N; m++){
    //    printf("index_and_color[%d]=index:%u color:%u\n",m,index_and_color[m].indice_orig,index_and_color[m].color);
    //}
    u32**chunks = (u32**)calloc(X, sizeof(u32*));
    //u32 * chunks[X];
    for (int j = 0; j<X; j++){
        //printf("alocando size para subarray de color_counts[%d]=%u\n", j,color_counts[j]);
        chunks[j] = (u32*)calloc(color_counts[j], sizeof(u32));
    }
    int amount;
    for (int i = 0; i<X;i++){
        amount = build_chunks( perm[i],index_and_color, N, chunks,i); //N^2?
        printf("Para el color %u encontré %d elementos\n",perm[i],amount);
    }
    // no hace falta aplanarlo !!
    //printf("Imprimiendo chunk aplanao...\n");
    u32 * flat_chunk = calloc(N,sizeof(u32));
    int count=0;
    for ( int k=0; k<X; k++){
        printf("k = %d\n",k);
        //printf(">\n");
        printf("color_counts[%d]=%u\n",k,color_counts[perm[k]]);
        for (int l=0; l<color_counts[perm[k]]; l++){
            printf("chunks[%d][%d]:%u\n",k,l,chunks[k][l]);
            //FijarOrden(p,G,chunks[k][l]);
            flat_chunk[count]=chunks[k][l];
            //printf("flatchunks[%d]:%u\n",count,chunks[k][l]);
            count++;
        }
    }

    printf("Imprimiendo chunk aplanao...\n");
    for (int i=0;i<N;i++){
        printf("flat_chunk[%d]=%u\n",i,flat_chunk[i]);
    }
    //chunks tiene las posiciones del orden interno en el nuevo orden perm
    // aplanamos chunks. Eso te da un arreglo de N posiciones en el orden interno.
    // con eso despues haces:
    for (u32 p=0; p<N; p++){
        //printf("flat_chunk[%u]=%u\n",p,flat_chunk[p]);
        printf("FijarOrden(%u,G, %u)\n",p,flat_chunk[p]);
        FijarOrden(p, G, flat_chunk[p]);
    }
    //falta memory managment de la concha de la lora
    return 1;
}




/*
char OrdenPorBloqueDeColores(Grafo G, u32 * perm){
    u32 X = Greedy(G);
    u32 N = NumeroDeVertices(G);
    u32 natural_array[N];
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
    calcular_natural_array(G,natural_array);
    countingSortByColor(natural_array,N, X, G);
    printf("Despues de counting sort....\n");
    for (int i=0; i<N; i++){
        printf("%u tiene color %u \n",Nombre(i,G), Color(i,G));
    }
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
*/

//0  1 2 3
//[0,1,2,3]
//perm = 3,2,1,0
//orden_interno = [2,3,1,0]
//
//fijarOrden(0,copia,3)
//G = [3,3,1,0]
