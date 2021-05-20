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
    int r = atoi(argv[5]);
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

    GResult * greedys_result = calloc(a,sizeof(GResult));
    for (int j=0; j< a; j++){
        greedys_result[a].result = 0;
        greedys_result[a].seed = 0;
        
    }
    printf("Te aviso que hago %d greedys...\n",a);
    for (int i = 0; i<a; i++){
        AleatorizarVertices(grafo,i);
        greedys_result[i].result = Greedy(grafo);
        greedys_result[i].seed = i;
        printf("seed %d greedy da %u\n",i, greedys_result[i].result);
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

    free(greedys_result);
    DestruccionDelGrafo(grafo);
    return 0;

}
