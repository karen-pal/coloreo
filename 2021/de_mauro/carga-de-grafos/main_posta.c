#include <stdio.h>
#include "RomaVictor.h"
#include "requeridas.h"
#include <stdbool.h>
#include <time.h>
#include <stdlib.h>

typedef
    struct ResultadoGreedy {
        u32 color_greedy;
        int semilla;

    }
    ResultadoGreedy;

int _gresult_compare(const void *_a, const void *_b) {
    ResultadoGreedy a = *(ResultadoGreedy *)_a;
    ResultadoGreedy b = *(ResultadoGreedy *)_b;
    if (a.color_greedy == b.color_greedy)
        return 0;
    else if (a.color_greedy < b.color_greedy)
        return -1;
    else
        return 1;
}

void AleatorizarPerm(u32 * perm, u32 R, u32 largo){
    srand(R);
    u32 *array_random = calloc(largo,sizeof(u32));
    if (array_random == NULL){
        return;
    }
    u32 i = 0;
    for (int k=0; k<largo;k++){
        array_random[k]=k;
    }
    for (u32 i = 0; i < largo * 10; i++) {
        u32 p1 = rand() % largo;
        u32 p2 = rand() % largo;
        u32 v1 = array_random[p1];
        u32 v2 = array_random[p2];
        array_random[p1] = v2;
        array_random[p2] = v1;
    }
    for (u32 i = 0; i < largo; i++) {
        perm[i] = array_random[i];
    }

    free(array_random);
    array_random=NULL;
    return;
}

int RandomEnRango(minimum_number, max_number){
    return ( rand() % (max_number + 1 - minimum_number) + minimum_number);
}

void SwapProbabilistico(u32 * perm, u32 largo, int e){
    for (int i=0; i<largo; i++) {
        if (RandomEnRango(0,1) < 1/e) {
            u32 to_swap = rand() % largo;
            u32 tmp = perm[i];
            perm[i] = perm[to_swap];
            perm[to_swap] = tmp;
        }
    }
}

int main(int argc, char *argv[]) {
    int contador;
    printf("El nombre del programa es: %s\n",argv[0]);
    if(argc==1){
        printf("\n No se ha pasado ningún parámetro");
        printf("\nSaliendo ...");
        return 1;
    }
    if(argc>=2)
    {
        printf("\nCantidad de parametros pasados: %d",argc);
        printf("\n----Mostrando los parametros pasados----");
        for(contador=0;contador<argc;contador++)
            printf("\nargv[%d]: %s",contador,argv[contador]);
    }
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
    char bipartito = Bipartito(grafo);
    printf("Es bipartito? %s\n",(bool)bipartito?"    >Si, es bipartito. Saliendo...":"    >No, no es.");
    if (bipartito) {
        return 0;
    }
    // paso 5
    //ordenamiento natural
    u32 * arreglo_natural = calloc(N,sizeof(u32));
    if (arreglo_natural == NULL) {
        return 0;
    }
    CalcularNaturalArray(grafo, arreglo_natural);

    for (int i= 0; i< N; i++){
        FijarOrden(i,grafo,arreglo_natural[i]);
    }
    free(arreglo_natural);
    ResultadoGreedy * resultado_greedy = calloc(a+1,sizeof(ResultadoGreedy));
    if (resultado_greedy == NULL) {
        return 0;
    }
    for (int j=0; j< a; j++){
        resultado_greedy[a].color_greedy = 0;
        resultado_greedy[a].semilla = 0;

    }
    for (int i = 0; i<a; i++){
        AleatorizarVertices(grafo,f+i);
        resultado_greedy[i].color_greedy = Greedy(grafo);
        cantidad_greedys++;
        resultado_greedy[i].semilla = f+i;
        printf("Con semilla %d greedy da %u\n",resultado_greedy[i].semilla, resultado_greedy[i].color_greedy);
    }
    qsort(resultado_greedy,a, sizeof(ResultadoGreedy),_gresult_compare);
    ResultadoGreedy mejor_resultado = resultado_greedy[0];
    int mejor_color = mejor_resultado.color_greedy;
    int mejor_semilla = mejor_resultado.semilla;
    printf("La mejor semilla fue %d que nos dio un coloreo con %d colores\n", mejor_semilla, mejor_color);
    AleatorizarVertices(grafo,mejor_semilla);
    printf("Corriendo greedy con esa semilla\n");
    u32 check = Greedy(grafo);
    cantidad_greedys++;

    // paso 6
    u32 * perm = calloc(mejor_color,sizeof(u32));
    if (perm == NULL) {
        return 0;
    }
    for (int i=0; i<mejor_color; i++){
        perm[i]=i;
    }
    u32 resultado_bloque;
    bool is_perm=true;
    for (u32 i = 0; i<b; i++){
        AleatorizarPerm(perm, f+i, mejor_color);
        OrdenPorBloqueDeColores(grafo, perm);
        resultado_bloque = Greedy(grafo);
        cantidad_greedys++;
        mejor_color = resultado_bloque;
        printf("Orden con perm aleatorizado con semilla %u dio como resultado %u\n",f+i, resultado_bloque);
        if (resultado_bloque>mejor_color){
            printf(">>> Error... %u > %u.\n> Saliendo ...", resultado_bloque,mejor_color);
            free(resultado_greedy);
            free(perm);
            DestruccionDelGrafo(grafo);
            return 1;
        }
    }
    AleatorizarPerm(perm, f+b-1, mejor_color);

    //paso 7
    Grafo copia1 = CopiarGrafo(grafo);
    Grafo copia2 = CopiarGrafo(grafo);
    u32 * copia1_perm = CopiarArreglo(perm, sizeof(u32),mejor_color);
    u32 * copia2_perm = CopiarArreglo(perm,sizeof(u32),mejor_color);
    u32 largo_copia1_perm = mejor_color;
    u32 largo_copia2_perm = mejor_color;
    u32 greedy_orig = 0;
    u32 greedy_copia1=mejor_color;
    u32 greedy_copia2=mejor_color;

    for (int i=0; i< c; i++){
        for (int j=0; j<d;j++){
            //🦠  Haciendo el aleatorio
            AleatorizarPerm(perm, j, mejor_color);
            OrdenPorBloqueDeColores(grafo, perm);
            greedy_orig = Greedy(grafo);
            cantidad_greedys++;
            mejor_color = greedy_orig;

            //😻  Haciendo el mayor/menor
            qsort(copia1_perm,largo_copia1_perm,sizeof(u32),_natural_compare);
            qsort(copia1_perm,greedy_copia1,sizeof(u32),_mayor_menor_comp);
            OrdenPorBloqueDeColores(copia1, copia1_perm);
            greedy_copia1 = Greedy(copia1);
            cantidad_greedys++;

            //🥺  Haciendo el probabilistico
            qsort(copia2_perm,largo_copia2_perm,sizeof(u32),_natural_compare);
            qsort(copia2_perm,greedy_copia2,sizeof(u32),_mayor_menor_comp);
            SwapProbabilistico(copia2_perm,greedy_copia2,e);
            OrdenPorBloqueDeColores(copia2,copia2_perm);
            greedy_copia2 = Greedy(copia2);
            cantidad_greedys++;

            printf("[Estrategia a]  Original con aleatorización y orden por bloque de colores: %u\n", greedy_orig);
            printf("[Estrategia b]  Copia1 con mayor a menor: %u\n", greedy_copia1);
            printf("[Estrategia c]  Copia2 con swap probabilístico: %u\n", greedy_copia2);
        }
    }
    free(resultado_greedy);
    free(perm);
    free(copia1_perm);
    free(copia2_perm);
    DestruccionDelGrafo(grafo);
    DestruccionDelGrafo(copia1);
    DestruccionDelGrafo(copia2);
    printf("Se hicieron en total %d greedys\n", cantidad_greedys);
    return 0;

}
