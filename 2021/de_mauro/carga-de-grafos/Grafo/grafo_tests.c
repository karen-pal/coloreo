
#include "grafo_desde_dimacs.c"
#include "../assert.h"

static Vertices vertices_carga_to_vertices_c(SortedSetU32 nombres, VerticesCarga vertices_carga) {
    Vertices result = vertices_carga_to_vertices_b(nombres, vertices_carga);
    sorted_set_u32_destroy(nombres);
    return result;
}

bool vertices_carga_test() {
    for (
        u32 numero_de_vertices = 1;
        numero_de_vertices < 4;
        numero_de_vertices = numero_de_vertices + 1
    ) {
        VerticesCarga vertices_carga = vertices_carga_comenzar(numero_de_vertices);

        for (u32 i = 0; i < numero_de_vertices; i = i + 1) {
            vertices_carga_agregar_lado(
                orden_natural_of(i),
                orden_natural_of((i + 1) % numero_de_vertices),
                vertices_carga
            );
        }

        Vertices vertices = vertices_carga_to_vertices_c(
            sorted_set_u32_from_array(
                array_u32_range(0, numero_de_vertices - 1)
            ),
            vertices_carga
        );

        for (u32 i = 0; i < numero_de_vertices; i = i + 1) {
            Assert(
                vertices_leer(orden_natural_of(i), vertices).has_value
            )
            Assert(
                // Es un ciclo
                vertices_leer(orden_natural_of(i), vertices).value.cantidad_de_vecinos == 2
            )
        }

        vertices_destroy(vertices);
    }

    EndTest
}

MaybeGrafoSt grafo_desde_archivo(c_string path) {
    FILE *file = fopen(path, "r");

    if (file == NULL) {
        return maybe_grafo_none();
    }

    DimacsParseResult parse_result = dimacs_from_file(file);

    fclose(file);

    if (!parse_result.is_success) {
        return maybe_grafo_none();
    }

    MaybeGrafoSt grafo = grafo_desde_dimacs_b(parse_result.success);

    return grafo;
}

bool grafo_grado_nombre_tests_0() {
    MaybeGrafoSt grafo = grafo_desde_archivo("grafos/C_3.col");

    Assert(grafo.has_value);

    for (u32 i = 0; i < 3; i = i + 1) {
        Assert(
            grafo_grado_vertice(orden_interno_of(i), grafo_borrow(grafo.value)).has_value
                && grafo_grado_vertice(orden_interno_of(i), grafo_borrow(grafo.value)).value == 2
        );
    }

    Assert(
        grafo_nombre_vecino(
            numero_de_vecino_of(0),
            orden_interno_of(0),
            grafo_borrow(grafo.value)
        ).value
            == 2
    );
    Assert(
        grafo_nombre_vecino(
            numero_de_vecino_of(1),
            orden_interno_of(0),
            grafo_borrow(grafo.value)
        ).value
            == 3
    );

    for (u32 i = 0; i < 3; i = i + 1) {
        Assert(
            grafo_nombre_vertice(orden_interno_of(i), grafo_borrow(grafo.value)).value == i + 1
        );
    }

    Assert(grafo.value.delta == 2);

    grafo_destroy(grafo.value);

    EndTest
}

bool grafo_grado_nombre_tests_1() {
    MaybeGrafoSt grafo = grafo_desde_archivo("grafos/KC");

    Assert(grafo.has_value);

    for (u32 i = 0; i < 100; i = i + 1) {
        Assert(
            grafo_grado_vertice(orden_interno_of(i), grafo_borrow(grafo.value)).has_value
                && grafo_grado_vertice(orden_interno_of(i), grafo_borrow(grafo.value)).value == 99
        );

        Assert(
            grafo_nombre_vertice(orden_interno_of(i), grafo_borrow(grafo.value)).value == i
        );
    }

    Assert(grafo.value.delta == 99);

    grafo_destroy(grafo.value);

    EndTest
}

bool grafo_apreton_de_manos_test() {
    char *grafos[3] = {
        "grafos/bxb15_22_10.col",
        "grafos/q10.corr",
        "grafos/school1.col"
    };

    for (u32 nro_grafo = 0; nro_grafo < 3; nro_grafo = nro_grafo + 1) {
        MaybeGrafoSt grafo = grafo_desde_archivo(grafos[nro_grafo]);

        Assert(grafo.has_value);

        u32 suma_de_grados = 0;

        for (u32 i = 0; true; i = i + 1) {
            maybe_u32 grado = grafo_grado_vertice(orden_interno_of(i), grafo_borrow(grafo.value));

            if (grado.has_value) {
                suma_de_grados = suma_de_grados + grado.value;
            } else {
                break;
            }
        }

        Assert(2 * grafo.value.numero_de_lados == suma_de_grados);

        grafo_destroy(grafo.value);
    }

    EndTest
}

bool grafo_copy_test() {
    MaybeGrafoSt grafo = grafo_desde_archivo("grafos/q10.corr");

    Assert(grafo.has_value);

    GrafoSt grafos[2] = {
        grafo.value,
        grafo_copy(grafo_borrow(grafo.value))
    };

    for (u32 i = 0; i < 2; i = i + 1) {
        Assert(
            grafo_fijar_color_vertice(
                color_of(i),
                orden_interno_of(0),
                grafo_mut_borrow(grafos[i])
            ) == SUCCESS
        );
        Assert(
            grafo_fijar_peso_lado(
                numero_de_vecino_of(0),
                orden_interno_of(0),
                peso_of(i),
                grafo_mut_borrow(grafos[i])
            ) == SUCCESS
        );
        Assert(
            grafo_fijar_orden_vertice(
                orden_interno_of(i + 2),
                orden_natural_of(i + 1),
                grafo_mut_borrow(grafos[i])
            ) == SUCCESS
        );
        Assert(
            grafo_fijar_orden_vertice(
                orden_interno_of(i + 1),
                orden_natural_of(i + 2),
                grafo_mut_borrow(grafos[i])
            ) == SUCCESS
        );
    }

    for (u32 i = 0; i < 2; i = i + 1) {
        Assert(
            grafo_color_vertice(
                orden_interno_of(0),
                grafo_borrow(grafos[i])
            ).value == i
        );
        Assert(
            grafo_peso_lado(
                numero_de_vecino_of(0),
                orden_interno_of(0),
                grafo_borrow(grafos[i])
            ).value == i
        );
        Assert(
            grafo_orden_vertice(
                orden_natural_of(i + 1),
                grafo_borrow(grafos[i])
            ).value == i + 2
        );
        Assert(
            grafo_orden_vertice(
                orden_natural_of(i + 2),
                grafo_borrow(grafos[i])
            ).value == i + 1
        );
    }

    grafo_destroy(grafos[0]);
    grafo_destroy(grafos[1]);

    EndTest
}

int main() {
    bool result = vertices_carga_test()
        && grafo_grado_nombre_tests_0()
        && grafo_grado_nombre_tests_1()
        && grafo_apreton_de_manos_test()
        && grafo_copy_test();

    return result ? 0 : -1;
}
