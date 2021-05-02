
/** A este archivo lo hago para probar si funciona bien la forma en que compilo y linkeo
 * el programa.
 * No me queda claro de la consigna cómo va a ser, por eso lo intenté hacer de la forma más simple que
 * se pueda: basta compilar el archivo `index.c` en un `.o` y linkearlo al momento de compilar otro archivo.
 */

#include "RomaVictor.h"
#include "requeridas.h"
#include <time.h>
double get_time() {
    return (double)clock()/CLOCKS_PER_SEC;
}




int main() {
    printf("\n");
    printf("Esperando grafo desde stdin...\n");
    printf("\n");

    Grafo grafo = ConstruccionDelGrafo();

    if (grafo == NULL) {
        fprintf(stderr, "error: No se puede leer el grafo\n");
        return -1;
    }

    //printf("Se leyó el grafo\n");
    //printf("\n");


    //printf("Número de vértices: %u\n", NumeroDeVertices(grafo));
    //printf("Número de lados: %u\n", NumeroDeLados(grafo));
    //printf("Delta: %u\n", Delta(grafo));
    //printf("\n");

    //printf("Colores antes \n");
    //printf("\n");
    //for (int i=0; i<NumeroDeVertices(grafo);i++){
    //    printf("Vertice: %i --- Color: %u\n", i, Color(i,grafo));
    //}
    //printf("Greedy \n");
    //printf("\n");
    u32 greedy = Greedy(grafo);
    printf("----> greedy %u\n", greedy);
    //u32 greedy3 = Greedy(grafo);
    //printf("----> greedy %u\n", greedy3);

    //printf("Colores despues \n");
    //printf("\n");
    //for (int i=0; i<NumeroDeVertices(grafo);i++){
    //    printf("Vertice: %i --- Color: %u\n", i, Color(i,grafo));
    //}

    u32 greedy2 = Greedy(grafo);
    printf("----> SEGUNDO greedy %u\n", greedy2);
    printf("Colores despues segundo greedy \n");
    //printf("\n");
    //for (int i=0; i<NumeroDeVertices(grafo);i++){
    //    printf("Vertice: %i --- Color: %u\n", i, Color(i,grafo));
    //}
    DestruccionDelGrafo(grafo);

    return 0;
}
