//   El otro main es similar pero con CopiarGrafo: deben copiar el grafo, 
//   hacerles Greedy y reordenes aleatorios a las dos copiaspor separado 
//   una cierta cantidad de veces, luego destruir la copia original, 
//   volver a copiar el grafo y repetir, imprimiendo loscolores obtenidos en cada Greedy
#include "RomaVictor.h"
#include "requeridas.h"
#include <stdbool.h>



int main() {
    printf("\n");
    printf("Cargando grafo desde stdin...\n");
    printf("\n");

    Grafo grafo = ConstruccionDelGrafo();
    Grafo copia = CopiarGrafo(grafo);
    if (grafo == NULL) {
        fprintf(stderr, "error: No se puede leer el grafo\n");
        return -1;
    }
    printf("Se leyó el grafo\n");
    printf("\n");

    printf("Inicialmente la copia daba %u con greedy\n", Greedy(copia));
    for (int i = 0; i<100; i++){
	AleatorizarVertices(grafo,i);
	u32 color_orig = Greedy(grafo);
	printf("----> con seed %i | greedy en orig: %u, greedy en la copia: %u\n",i,color_orig, Greedy(copia));
    }

    DestruccionDelGrafo(copia);
    printf("Destruyendo primera copia\n");
    Grafo copia2 = CopiarGrafo(grafo);
    printf("Inicialmente la copia2 daba %u con greedy\n", Greedy(copia2));
    for (int i = 0; i<100; i++){
	AleatorizarVertices(grafo,i);
	u32 color_orig = Greedy(grafo);
	printf("----> con seed %i | greedy en orig: %u, greedy en la copia: %u\n",i,color_orig, Greedy(copia2));
    }
    DestruccionDelGrafo(copia2);
    DestruccionDelGrafo(grafo);

    return 0;
}
