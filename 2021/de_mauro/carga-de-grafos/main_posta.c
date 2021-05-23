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

int randomInRange(minimum_number, max_number){
    return ( rand() % (max_number + 1 - minimum_number) + minimum_number);
}

void probabilisticSwap(u32 * perm, u32 length, int e){
    for (int i=0; i<length; i++) {
        if (randomInRange(0,1) < 1/e) {
            u32 to_swap = rand() % length;
            u32 tmp = perm[i];
            perm[i] = perm[to_swap];
            perm[to_swap] = tmp;
        }
    }
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
    int cantidad_greedys=0;
    Grafo grafo = ConstruccionDelGrafo();
    printf("\n");
    printf("Grafo cargado desde stdin...\n");
    printf("\n");
    // paso 2
    u32 N = NumeroDeVertices(grafo);
    printf("Número de vértices: %u\n", N);
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
    //ordenamiento natural
    u32 * natural_array = calloc(N,sizeof(u32));
    calcular_natural_array(grafo, natural_array);

    for (int i= 0; i< N; i++){
        FijarOrden(i,grafo,natural_array[i]);
    }
    free(natural_array);
    GResult * greedys_result = calloc(a+1,sizeof(GResult));
    for (int j=0; j< a; j++){
        greedys_result[a].result = 0;
        greedys_result[a].seed = 0;

    }
    //printf("Te aviso que hago %d greedys...\n",a);
    for (int i = 0; i<a; i++){
        AleatorizarVertices(grafo,f+i);
        greedys_result[i].result = Greedy(grafo);
        cantidad_greedys++;
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
    cantidad_greedys++;
    //printf("Debería ser que %d (encontrado iterando)\n     es igual a %u\n", best_X,check);
    // paso 6
    u32 * perm = calloc(best_X,sizeof(u32));
    for (int i=0; i<best_X; i++){
        perm[i]=i;
    }
    u32 bloque_result;
    //printf("Te muestro perm original........\n");
    //for(int j=0; j<best_X; j++){
    //    printf("perm[%d]=%u\n",j,perm[j]);
    //}
    bool is_perm=true;
    for (u32 i = 0; i<b; i++){
        //printf("i=%u ---- b=%d\n",i,b);
        AleatorizarPerm(perm, f+i, best_X);
        OrdenPorBloqueDeColores(grafo, perm);
        bloque_result = Greedy(grafo);
        cantidad_greedys++;
        best_X = bloque_result;
        printf("Orden con perm %u dio como resultado %u\n",f+i, bloque_result);
        printf("despues de greedy y reasignar best_X\n");
        if (bloque_result>best_X){
            printf(">>> Error... %u > %u.\n> Exiting ...", bloque_result,best_X);
            free(greedys_result);
            free(perm);
            DestruccionDelGrafo(grafo);
            return 1;
        }
    }
    AleatorizarPerm(perm, f+b-1, best_X);
    //paso 7
    //is_perm = check_permutation(perm,best_X);
    //if (!is_perm){
    //    for(int l = 0; l<best_X; l++){
    //        printf("perm[%d]=%d\n", l, perm[l]);
    //    }
    //}
    Grafo copia1 = CopiarGrafo(grafo);
    Grafo copia2 = CopiarGrafo(grafo);
    u32 * copia1_perm = copiar_arreglo(perm, sizeof(u32),best_X);
    u32 * copia2_perm = copiar_arreglo(perm,sizeof(u32),best_X);
    u32 largo_copia1_perm = best_X;
    u32 largo_copia2_perm = best_X;
    int count=0;
    u32 greedy_orig = 0;
    u32 greedy_copia1=best_X;
    u32 greedy_copia2=best_X;

    //printf(">>>>>copia1_perm original,antes que ningun reorden %d\n");
    //for(int l = 0; l<largo_copia1_perm; l++){
    //    printf("copia1_perm[%d]=%d\n", l, copia1_perm[l]);
    //}

    for (int i=0; i< c; i++){
        for (int j=0; j<d;j++){
            printf("Loop %d / %d\n", count, c*d);
            count++;

            //printf("🦠  Haciendo el aleatorio\n");
            AleatorizarPerm(perm, j, best_X);
            OrdenPorBloqueDeColores(grafo, perm);
            greedy_orig = Greedy(grafo);
            cantidad_greedys++;
            best_X = greedy_orig;

            //printf("😻  Haciendo el mayor/menor\n");
            qsort(copia1_perm,largo_copia1_perm,sizeof(u32),_natural_compare);
            qsort(copia1_perm,greedy_copia1,sizeof(u32),_mayor_menor_comp);
            OrdenPorBloqueDeColores(copia1, copia1_perm);
            greedy_copia1 = Greedy(copia1);
            cantidad_greedys++;

            //printf("🥺  Haciendo el probabilistico\n");
            qsort(copia2_perm,largo_copia2_perm,sizeof(u32),_natural_compare);
            qsort(copia2_perm,greedy_copia2,sizeof(u32),_mayor_menor_comp);
            probabilisticSwap(copia2_perm,greedy_copia2,e);
            OrdenPorBloqueDeColores(copia2,copia2_perm);
            greedy_copia2 = Greedy(copia2);
            cantidad_greedys++;

            printf("🦠  Orig con aleat y orden por bloque de colores: %u\n", greedy_orig);
            printf("😻 Copia1 con mayor a menor: %u\n", greedy_copia1);
            printf("🥺  Copia2 con swaps: %u\n", greedy_copia2);
        }
    }
    free(greedys_result);
    free(perm);
    free(copia1_perm);
    free(copia2_perm);
    DestruccionDelGrafo(grafo);
    DestruccionDelGrafo(copia1);
    DestruccionDelGrafo(copia2);
    printf("Se hicieron en total %d greedys... Chau\n", cantidad_greedys);
    return 0;

}
