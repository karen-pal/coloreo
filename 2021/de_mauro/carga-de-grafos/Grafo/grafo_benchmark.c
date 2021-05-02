
#include "grafo_desde_dimacs.c"
#include "../assert.h"

// Este archivo sirve para medir tiempos de carga de los grafos más grandes.

// En mi compu ninguno tardó más de 20 segundos.

// Naturalmente, este archivo se ejecuta sin Valgrind, y por eso no es muy
// confiable para testear otras cosas aparte del tiempo.

bool grafo_benchmark(c_string path) {
    printf("\n");
    printf("--- GRAFO BENCHMARK ----------------------------------\n");
    printf("\n");
    printf("Grafo: %s\n", path);
    printf("\n");

    FILE *file = fopen(path, "r");

    Assert(file != NULL);

    double time0 = get_time();

    DimacsParseResult parse_result = dimacs_from_file(file);

    double duracion_dimacs = get_time() - time0;

    printf("dimacs_from_file: %f segundos\n", duracion_dimacs);

    fclose(file);

    Assert(parse_result.is_success);

    double time1 = get_time();

    MaybeGrafoSt grafo = grafo_desde_dimacs_b(parse_result.success);

    double duracion_grafo = get_time() - time1;

    Assert(grafo.has_value);

    printf("grafo_desde_dimacs: %f segundos\n", duracion_grafo);
    printf("\n");
    printf("Total: %f segundos\n", duracion_dimacs + duracion_grafo);
    printf("\n");

    double time2 = get_time();

    GrafoSt grafo2 = grafo_copy(grafo_borrow(grafo.value));
    grafo_destroy(grafo.value);

    double duracion_copy = get_time() - time2;

    printf("grafo_copy: %f segundos\n", duracion_copy);
    printf("\n");

    grafo_destroy(grafo2);

    // El tiempo de carga debe ser razonable.
    Assert(duracion_dimacs + duracion_grafo + duracion_copy < 2 * 60);

    EndTest
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("\n");
        printf("--- GRAFO BENCHMARK ----------------------------------\n");
        printf("\n");
        printf("Ejemplo de uso: %s grafos/catedra/Octo.txt\n", argv[0]);
        printf("\n");
        return 0;
    }

    bool result = grafo_benchmark(argv[1]);

    printf("\n");

    return result ? 0 : -1;
}
