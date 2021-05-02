
/** A este archivo lo hago para probar si funciona bien la forma en que compilo y linkeo
 * el programa.
 * No me queda claro de la consigna cómo va a ser, por eso lo intenté hacer de la forma más simple que
 * se pueda: basta compilar el archivo `index.c` en un `.o` y linkearlo al momento de compilar otro archivo.
 */

#include "RomaVictor.h"
#include "requeridas.h"
#include <time.h>

void imprimir_estadisticas_de_vertice(u32 i, Grafo grafo) {
    printf("  Nombre: %u\n", Nombre(i, grafo));
    printf("  Color: %u\n", Color(i, grafo));
    printf("  Grado: %u\n", Grado(i, grafo));
    printf("  Del primer vecino:\n");
    printf("    Nombre: %u\n", NombreVecino(0, i, grafo));
    printf("    Color: %u\n", ColorVecino(0, i, grafo));
    printf("    Orden: %u\n", OrdenVecino(0, i, grafo));
    printf("    PesoLadoConVecino: %u\n", PesoLadoConVecino(0, i, grafo));
    printf("  Del último vecino:\n");
    printf("    Nombre: %u\n", NombreVecino(Grado(i, grafo) - 1, i, grafo));
    printf("    Color: %u\n", ColorVecino(Grado(i, grafo) - 1, i, grafo));
    printf("    Orden: %u\n", OrdenVecino(Grado(i, grafo) - 1, i, grafo));
    printf("    PesoLadoConVecino: %u\n", PesoLadoConVecino(Grado(i, grafo) - 1, i, grafo));
}

/// Devuelve el tiempo en segundos
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

    printf("Se leyó el grafo\n");
    printf("\n");


    printf("Número de vértices: %u\n", NumeroDeVertices(grafo));
    printf("Número de lados: %u\n", NumeroDeLados(grafo));
    printf("Delta: %u\n", Delta(grafo));
    printf("\n");

    printf("\n");
    printf("Greedy \n");
    printf("\n");
    u32 greedy = Greedy(grafo);
    printf("----> greedy %u\n", greedy);
    DestruccionDelGrafo(grafo);

    return 0;
}
