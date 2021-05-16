#include "RomaVictor.h"
#include "requeridas.h"
#include <stdbool.h>

int main() {
    printf("\n");
    printf("Cargando grafo desde stdin...\n");
    printf("\n");

    Grafo grafo = ConstruccionDelGrafo();
    char bip_principio = Bipartito(grafo);
    printf("Al principio, es bipartito? %d\n",(bool)bip_principio);
    int seed;
    u32 color_minimo = (2)^32 - 1 ;
    if (grafo == NULL) {
        fprintf(stderr, "error: No se puede leer el grafo\n");
        return -1;
    }
    printf("Se leyó el grafo\n");
    printf("\n");

    int color_actual;
    for (int i = 0; i<1000; i++){
	AleatorizarVertices(grafo,i);
	color_actual = Greedy(grafo);
	if (color_actual < color_minimo){
            color_minimo=color_actual;
	    seed = i;
	}
	//printf("----> %d greedy %u\n",i, Greedy(grafo));
    }
    printf("La seed %i da greedy con colo minimo de %d\n",seed, color_minimo);
    char bip_final = Bipartito(grafo);
    printf("Es bipartito despues? %d\n",(bool)bip_final);
    printf("Se mantuvo inalterado el valor de bipartito con reordenes? %s\n", (bip_final == bip_principio)?"true":"false");
    DestruccionDelGrafo(grafo);

    return 0;
}
