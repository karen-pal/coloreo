#include <stdio.h>
#include "RomaVictor.h"
#include "requeridas.h"
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

typedef
    struct GResult {
        u32 result;
        int seed;

    }
    GResult;

int _gresult_compare(const void *_a, const void *_b) {
    GResult a = *(GResult *)_a;
    GResult b = *(GResult *)_b;
    if (a.result == b.result)
        return 0;
    else if (a.result < b.result)
        return -1;
    else
        return 1;
}

void AleatorizarPerm(u32 * perm, u32 R, u32 length){
    srand(R);
    u32 *array_random = calloc(length,sizeof(u32));
    if (array_random == NULL){
        return;
    }
    u32 i = 0;
    for (int k=0; k<length;k++){
        array_random[k]=k;
    }
    for (u32 i = 0; i < length * 10; i++) {
        u32 p1 = rand() % length;
        u32 p2 = rand() % length;
        u32 v1 = array_random[p1];
        u32 v2 = array_random[p2];
        array_random[p1] = v2;
        array_random[p2] = v1;
    }
    for (u32 i = 0; i < length; i++) {
        perm[i] = array_random[i];
    }

    free(array_random);
    array_random=NULL;
    return;
}

int main(int argc, char *argv[]) {
    int counter;
    printf("Program Name Is: %s\n",argv[0]);
    if(argc==1){
        printf("\nNo Extra Command Line Argument Passed Other Than Program Name");
        printf("\nExiting ...");
        return 1;
    }
    if(argc>=2)
    {
        printf("\nNumber Of Arguments Passed: %d",argc);
        printf("\n----Following Are The Command Line Arguments Passed----");
        for(counter=0;counter<argc;counter++)
            printf("\nargv[%d]: %s",counter,argv[counter]);
    }
    int trash = argv[0];
    int a = atoi(argv[1]);
    int b = atoi(argv[2]);
    int c = atoi(argv[3]);
    int d = atoi(argv[4]);
    int e = atoi(argv[5]);
    int f = atoi(argv[6]);
    Grafo grafo = ConstruccionDelGrafo();
    printf("\n");
    printf("Grafo cargado desde stdin...\n");
    printf("\n");
    // paso 2
    printf("Número de vértices: %u\n", NumeroDeVertices(grafo));
    printf("Número de lados: %u\n", NumeroDeLados(grafo));
    printf("Delta: %u\n", Delta(grafo));
    // paso 3
    char bipartite = Bipartito(grafo);
    printf("Es bipartito? %s\n",(bool)bipartite?"    >Si, es bipartito. Exiting...":"    >No, no es.");
    if (bipartite) {
        return 0;
    }
    // paso 4
    //ya taba
    // paso 5
    //TODO: ordenamiento natural
    GResult * greedys_result = calloc(a,sizeof(GResult));
    for (int j=0; j< a; j++){
        greedys_result[a].result = 0;
        greedys_result[a].seed = 0;
        
    }
    printf("Te aviso que hago %d greedys...\n",a);
    for (int i = 0; i<a; i++){
        AleatorizarVertices(grafo,f+i);
        greedys_result[i].result = Greedy(grafo);
        greedys_result[i].seed = f+i;
        printf("seed %d greedy da %u\n",greedys_result[i].seed, greedys_result[i].result);
    }
    qsort(greedys_result,a, sizeof(GResult),_gresult_compare);
    GResult best_result = greedys_result[0];
    int best_X = best_result.result;
    int best_seed = best_result.seed;
    printf("La mejor seed fue %d que nos dio un coloreo con %d colores\n", best_seed, best_X);
    AleatorizarVertices(grafo,best_seed);
    printf("Corriendo greedy con este seed excelente..\n");
    u32 check = Greedy(grafo);
    printf("Debería ser que %d (encontrado iterando)\n     es igual a %u\n", best_X,check);
    // paso 6
    u32 * perm = calloc(best_X,sizeof(u32));
    for (int i=0; i<best_X; i++){
        perm[i]=i;
    }
    u32 bloque_result;
    for (u32 i = 0; i<b; i++){
        AleatorizarPerm(perm, f+i, best_X);
        printf("Te muestro perm........\n");
        //for(int j=0; j<best_X; j++){
        //    printf("perm[%d]=%u\n",j,perm[j]);
        //}
        OrdenPorBloqueDeColores(grafo, perm);
        bloque_result = Greedy(grafo);
        printf("Orden con perm %u dio como resultado %u\n",f+i, bloque_result);
        if (bloque_result>best_X){
            printf("Error... %u > %u.\n> Exiting ...", bloque_result,best_X);
            return 1;
        }
    }
    

    free(greedys_result);
    free(perm);
    DestruccionDelGrafo(grafo);
    return 0;

}
