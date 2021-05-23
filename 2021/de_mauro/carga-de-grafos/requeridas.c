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
        u32 indice = OrdenVecino(j,x,G);
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

void ArregloColoresVecino(u32 vertice, Grafo G, u32 * color_vecinos, u32 grado){
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

u32 EncontrarMinimoColor(u32 * color_vecinos, u32 grado){
    u32 color = 0;
    qsort(color_vecinos,grado, sizeof(u32),_natural_compare);
    if (color_vecinos[0]>0) return color;
    for (u32 j=0; j<grado; j++){
        if (color_vecinos[j] == color + 1) {
            color++;
        } else  if (color_vecinos[j] != color) {
            break;
        }
    }
    color++;
    return color;

}

u32 Greedy(Grafo G) {
    u32 max_color = 0;
    u32 N = NumeroDeVertices(G);
    for (u32 j = 0; j < N; j++) {
        FijarColor(-1, j,G);
    }
    FijarColor(0,0,G);
    u32 grado;
    for (u32 i = 1; i < N; i++) {
        grado = Grado(i,G);
    	u32 * color_vecinos = malloc(grado* sizeof(u32));
        if (color_vecinos == NULL) {
            return 0;
        }
	    ArregloColoresVecino(i,G, color_vecinos, grado);
	    u32 min_color = EncontrarMinimoColor(color_vecinos, grado);

        if (min_color > max_color) {
            max_color = min_color;
        }
        FijarColor(min_color, i, G);
	    free(color_vecinos);
    }
    return max_color + 1;
}

char Bipartito(Grafo G) {
    u32 N = NumeroDeVertices(G);
    u32 *vertices_cc = calloc(N,sizeof(u32));
    if (vertices_cc == NULL){
        return 0;
    }
    for(u32 i = 0; i < N; i++) {
        if (vertices_cc[i] == 0) {
            Bfs(i, vertices_cc, G);
        }
    }
    for(u32 i = 0; i < N; i++) {
        FijarColor(vertices_cc[i]%2,i,G);
    }
    int color = 2;
    for(u32 i = 0; i < N; i++) {
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
    for (u32 k=0; k<N;k++){
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

u32 maximo(u32 a, u32 b) {
    return a < b ? b : a;
}

u32* CopiarArreglo(void *orig, size_t tamano_elemento, u32 largo) {
    u32 capacidad = maximo(8, largo);

    void *data = calloc(capacidad, tamano_elemento);
    if (data == NULL) {
        return 0;
    }
    memcpy(data, orig, largo * tamano_elemento);

    return data;
}


int _mayor_menor_compare(const void *_a, const void *_b) {
    u32 a = *(u32 *)_a;
    u32 b = *(u32 *)_b;
    if (a == b)
        return 0;
    else if (a > b)
        return -1;
    else
        return 1;
}
bool ChequearPermutacion ( u32* perm, u32 color){
    u32* perm_ord = CopiarArreglo(perm, sizeof(u32), color);
    qsort(perm_ord, color, sizeof(u32), _natural_compare);
    for (u32 i=0; i<color; i++){
        if (perm_ord[i] != i){
            printf("Error en permutacion:deberia ser %d en vez de %d\n", perm_ord[i],i);
            free(perm_ord);
                return false;
        }
    }
    free(perm_ord);
    return true;
}
int _struct_compare(const void *_a, const void *_b) {
    Bookkeeping a = *(Bookkeeping *)_a;
    Bookkeeping b = *(Bookkeeping *)_b;
    if (a.nombre_nodo == b.nombre_nodo)
        return 0;
    else if (a.nombre_nodo < b.nombre_nodo)
        return -1;
    else
        return 1;
}
void CalcularNaturalArray(Grafo G, u32 * book_keeping){
    u32 N = NumeroDeVertices(G);
    Bookkeeping * vertices = calloc(N, sizeof(Bookkeeping));
    if (vertices == NULL) {
        return;
    }
    for (u32 i=0; i< N; i++) {
        vertices[i].nombre_nodo = Nombre(i,G);
        vertices[i].indice_interno = i;
    }
    //ordenamos por Nombre. Despues recuperamos los i.
    qsort(vertices, N, sizeof(Bookkeeping), _struct_compare);
    for (u32 index=0; index<N;index++) {
        book_keeping[vertices[index].indice_interno] = index; //(index, color)
    }
    free(vertices);
    //i = 2, Nombre(i,G) = 9
    //ordenar y queda
    //[(9,2), ...] //vertices
    //implica que el vertice 2 del orden interno està en la posicion 0 del ord natural
    //si quiero ver en qué pos nat está el vértice con posicion 5 en el orden interno
    //entonces eso me lo da hacer
    //book_keeping[5]
    //book_keeping te hace el mapping ord_interno->orden_natural
}


void ConstruirArreglo(Grafo G, Elem * indice_color, u32 * conteo_colores){
    for (u32 i=0; i<NumeroDeVertices(G);i++){
        indice_color[i].indice_orig = i;
        indice_color[i].color = Color(i,G);
        conteo_colores[Color(i,G)]++;
    }
}

u32 ConstruirChunks(u32 color,Elem * indice_color,u32 N,u32** chunks,int i, u32 conteo_color){
    u32 cantidad = 0;
    for (u32 j=0; j<N; j++){
        if (indice_color[j].color == color) {
            chunks[i][cantidad] =indice_color[j].indice_orig;
            cantidad++;
        }
    }
    if (cantidad != conteo_color){
        printf("Error al construir chunks! Tendria que haber encontrado conteo_color %u de color\
         %u pero recorri %d\n",conteo_color,color,cantidad);
    }
    return cantidad;
}


char OrdenPorBloqueDeColores(Grafo G, u32 * perm){
    u32 N = NumeroDeVertices(G);
    u32 * arreglo_natural = calloc(N,sizeof(u32));
    if (arreglo_natural == NULL) {
        return 0;
    }
    CalcularNaturalArray(G, arreglo_natural);
    //Dejar el orden interno como el orden natural,
    // para evitar confusiones entre nombres y posiciones internas
    for (u32 i= 0; i<N; i++){
        FijarOrden(i,G,arreglo_natural[i]);
    }
    u32 X = MaxColor(G)+1;
    bool is_perm = ChequearPermutacion(perm, X);
    if (!is_perm){
        printf("Error. No es permutación");
        return 0;
    }
    Elem * indice_color = calloc(N, sizeof(Elem));
    u32 * conteo_colores = calloc(X, sizeof(u32));
    if (indice_color == NULL || conteo_colores == NULL) {
        return 0;
    }
    for (u32 i=0; i<X;i++){
        conteo_colores[i]=0;
    }
    ConstruirArreglo(G, indice_color, conteo_colores);
    u32 ** chunks = (u32**)calloc(X, sizeof(u32*));
    if (chunks == NULL) {
        return 0;
    }
    for (u32 j = 0; j<X; j++){
        chunks[j] = calloc(conteo_colores[perm[j]], sizeof(u32));
        if (chunks[j] == NULL) {
            return 0;
        }
    }
    for (u32 i = 0; i<X;i++){
        ConstruirChunks( perm[i],indice_color, N, chunks,i,conteo_colores[perm[i]]);
    }
    u32 count=0;
    //chunks tiene las posiciones del orden interno en el nuevo orden perm
    for (u32 k=0; k<X; k++){
        for (u32 l=0; l<conteo_colores[perm[k]]; l++){
            FijarOrden(count,G,arreglo_natural[chunks[k][l]]);
            count++;
        }
    }
    for (u32 j=0; j<X;j++){
        free(chunks[j]);
    }
    free(chunks);
    free(indice_color);
    free(conteo_colores);
    free(arreglo_natural);
    return 1;
}
