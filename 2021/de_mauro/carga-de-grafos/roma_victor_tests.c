
/** A este archivo lo hago para probar si funciona bien la forma en que compilo y linkeo
 * el programa.
 * No me queda claro de la consigna cómo va a ser, por eso lo intenté hacer de la forma más simple que
 * se pueda: basta compilar el archivo `index.c` en un `.o` y linkearlo al momento de compilar otro archivo.
 */

#include "RomaVictor.h"
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

    Grafo grafo2 = CopiarGrafo(grafo);

    printf("Número de vértices: %u\n", NumeroDeVertices(grafo));
    printf("Número de lados: %u\n", NumeroDeLados(grafo));
    printf("Delta: %u\n", Delta(grafo));
    printf("\n");

    printf("Del primer vértice:\n");
    imprimir_estadisticas_de_vertice(0, grafo);
    printf("\n");

    printf("Del último vértice:\n");
    imprimir_estadisticas_de_vertice(NumeroDeVertices(grafo) - 1, grafo);
    printf("\n");

    // --- Probamos editar un vértice cualquiera

    // Antes de editarlo, lo ponemos en orden 0 así de paso probamos el
    // FijarOrden

    u32 color = (u32)(get_time() * 2015) % NumeroDeVertices(grafo);
    u32 orden = (u32)(get_time() * 1238) % NumeroDeVertices(grafo);
    u32 peso = (u32)(get_time() * 5478) % NumeroDeVertices(grafo);

    for (u32 i = 0; i < NumeroDeVertices(grafo); i = i + 1) {
        FijarOrden(i, grafo, i);
    }
    FijarOrden(0, grafo, orden);
    FijarOrden(orden, grafo, 0);

    FijarColor(color, 0, grafo);
    FijarPesoLadoConVecino(0, 0, peso, grafo);


    printf("Del vértice nro %u (editado):\n", orden);
    imprimir_estadisticas_de_vertice(0, grafo);
    printf("\n");

    /** Por defecto, el Color = Orden y Peso = 0
     * Al ver los resultados de las estadísticas del vértice nro `orden`
     * hay que verificar que:
     * 1. Color != orden
     * 2. PesoLadoConVecino del primer vecino != 0 (aunque puede dar 0)
    */

   printf("De la copia del vértice nro %u:\n", orden);
   imprimir_estadisticas_de_vertice(orden, grafo2);
   printf("\n");

    DestruccionDelGrafo(grafo);
    DestruccionDelGrafo(grafo2);

    return 0;
}
