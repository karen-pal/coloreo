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
    //printf("Te muestro la cosa despues de ordenar........\n");
    //for(int j=0; j<color; j++){
    //    printf("perm[%d]=%u\n",j,perm_ord[j]);
    //}
    for (int i=0; i<color; i++){
        if (perm_ord[i] != i){
            printf("perm tiene un %d donde debería ir %d\n", perm_ord[i],i);
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
    Result * vertices = calloc(N, sizeof(Result));
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
    free(vertices);
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


void buildArray(Grafo G, Elem * index_and_color, u32 * color_counts){
    for (int i=0; i<NumeroDeVertices(G);i++){
        index_and_color[i].indice_orig = i;
        index_and_color[i].color = Color(i,G);
        color_counts[Color(i,G)]++;
    }
}

int build_chunks(u32 color,Elem * index_and_color,u32 N,u32** chunks,int i, u32 color_count){
    int amount = 0;
    //for (int k = 0; k<3; k++){
    //    printf("en chunks[%d]=%u\n",k,chunks[k]);
    //}
    //printf(">>>Considerando color %u\n",color);
    for (int j=0; j<N; j++){
        if (index_and_color[j].color == color) {
            //printf("color de perm %u, \n",color);
            //printf("escribiendo en chunks[%d][%d] = %u\n",i,amount,index_and_color[j].indice_orig);
            chunks[i][amount] =index_and_color[j].indice_orig;
            amount++;
        }
    }
    if (amount != color_count){
        printf(">>>>>>>>>>>> No escribí todo, está mal!\n");
        printf("color de perm %u tenia que ser %u pero escribí %d\n",color,color_count,amount);
    }
    return amount;
}


char OrdenPorBloqueDeColores(Grafo G, u32 * perm){
    u32 N = NumeroDeVertices(G);
    u32 * natural_array = calloc(N,sizeof(u32));
    calcular_natural_array(G, natural_array);

    //Dejar el orden interno como el orden natural,
    // para evitar confusiones entre nombres y posiciones internas
    for (int i= 0; i< N; i++){
        FijarOrden(i,G,natural_array[i]);
    }

    u32 X = MaxColor(G)+1;
    printf("Adentro de ordenporbloquedecolores\n X = %u\n",X);
    //for (u32 i=0; i<NumeroDeVertices(G);i++){
    //    printf("Vertice  %u ---- Color: %u\n", Nombre(i,G), Color(i,G));
    //}
    bool is_perm = check_permutation(perm, X);
    printf("is perm? %s\n",is_perm?"true":"false");
    if (!is_perm){
        return 0;
    }
    Elem * index_and_color = calloc(N,sizeof(Elem));
    u32 * color_counts = calloc(X, sizeof(u32));
    for (int i=0; i<X;i++){
        color_counts[i]=0;
    }
    buildArray(G, index_and_color, color_counts);
    printf("Construí arreglo\n");

    //for (int m=0; m<X; m++){
    //    printf("color_counts[%d]=%u\n",m,color_counts[m]);
    //}
    //for (int m=0; m<N; m++){
    //    printf("index_and_color[%d]=index:%u color:%u\n",m,index_and_color[m].indice_orig,index_and_color[m].color);
    //}
    u32 ** chunks = (u32**)calloc(X, sizeof(u32*));
    printf("Terminé de pedir espacio para chunks\n");
    for (int j = 0; j<X; j++){
        //printf("alocando size para subarray de color_counts[%d]=%u\n", j,color_counts[j]);
        chunks[j] = calloc(color_counts[perm[j]], sizeof(u32));
    }
    printf("Terminé de pedir espacio para interchunks\n");
    int amount=0;
    int ejecuciones=0;
    for (int i = 0; i<X;i++){
        //printf("Llamando con i=%d, color perm[i] =%d, limite para llegar es X=%u \n",i,perm[i],X);
        //printf("por llamar a build chunks con color %u\n",perm[i]);
        amount = build_chunks( perm[i],index_and_color, N, chunks,i,color_counts[perm[i]]); //N^2?
        ejecuciones++;
        //printf("Para el color %u encontré %d elementos\n",perm[i],amount);
    }
    //printf("Se ejecutó eso %d veces... diferencia con N*N de %d\n", ejecuciones, N*N-ejecuciones);
    // no hace falta aplanarlo !!
    //printf("haciendo chunk aplanao...\n");
    u32 * flat_chunk = calloc(N,sizeof(u32));
    int count=0;
    for ( int k=0; k<X; k++){
        //printf("k = %d\n",k);
        //printf(">\n");
        //printf("color_counts[%d]=%u\n",k,color_counts[perm[k]]);
        for (int l=0; l<color_counts[perm[k]]; l++){
            //printf("chunks[%d][%d]:%u\n",k,l,chunks[k][l]);
            //FijarOrden(p,G,chunks[k][l]);
            flat_chunk[count]=chunks[k][l];
            //printf("flatchunks[%d]:%u\n",count,chunks[k][l]);
            count++;
        }
    }

    //for (int i=0;i<N;i++){
    //    printf("flat_chunk[%d]=%u\n",i,flat_chunk[i]);
    //}
    //chunks tiene las posiciones del orden interno en el nuevo orden perm
    // aplanamos chunks. Eso te da un arreglo de N posiciones en el orden interno.
    // con eso despues haces:
    for (u32 p=0; p<N; p++){
        //printf("flat_chunk[%u]=%u\n",p,flat_chunk[p]);
        //printf("FijarOrden(%u,G, %u)\n",p,flat_chunk[p]);
        FijarOrden(p, G, natural_array[flat_chunk[p]]);
    }
    //free memory
    //por alguna razón lo siguiente falla
    for (int j=0; j<X;j++){
        free(chunks[j]);
    }
    free(chunks);
    free(flat_chunk);
    free(index_and_color);
    free(color_counts);
    free(natural_array);
    //falta memory managment de la concha de la lora?
    return 1;
}

