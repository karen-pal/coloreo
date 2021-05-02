
#include "RomaVictor.h"

int main() {
    Grafo grafo = ConstruccionDelGrafo();

    if (grafo == NULL) {
        fprintf(stderr, "Grafo NULL\n");
        return 1;
    }

    printf("ConstruccionDelGrafo: N=%u M=%u D=%u\n", NumeroDeVertices(grafo), NumeroDeLados(grafo), Delta(grafo));

    DestruccionDelGrafo(grafo);

    return 0;
}
