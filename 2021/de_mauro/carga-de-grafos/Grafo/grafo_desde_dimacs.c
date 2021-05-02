#ifndef GRAFO_DESDE_DIMACS
#define GRAFO_DESDE_DIMACS

#include "GrafoSt.c"
#include "../Dimacs/Dimacs.c"
#include "../Nombres/Nombres.c"

// --- CARGA DE VERTICES

/**
 * Esta estructura me parece innecesaria si calculo la cantidad de vecinos de cada vértice
 * de antemano. Hubiera sido más fácil hacerlo de esa forma (y da lugar a optimizar un poco),
 * pero no lo pensé antes, y ahora ya está.
 */

typedef
    struct VerticesCarga {
        ArrayU32 *vecinos;
    }
    VerticesCarga;

typedef VerticesCarga vertices_carga_borrowed;

void vertices_carga_destroy(VerticesCarga vertices_carga) {
    free(vertices_carga.vecinos);
}

VerticesCarga vertices_carga_comenzar(u32 cantidad_de_vertices) {
    VerticesCarga vertices_carga;

    vertices_carga.vecinos = (ArrayU32 *)safe_calloc(cantidad_de_vertices, sizeof(ArrayU32));

    for (u32 i = 0; i < cantidad_de_vertices; i = i + 1) {
        vertices_carga.vecinos[i] = array_u32_empty();
    }

    return vertices_carga;
}

/** Pre: i, j < numero_de_vertices(vertices_carga) */
VerticesCarga vertices_carga_agregar_lado(orden_natural_t i, orden_natural_t j, VerticesCarga vertices_carga) {
    vertices_carga.vecinos[i.value] = array_u32_push(j.value, vertices_carga.vecinos[i.value]);
    vertices_carga.vecinos[j.value] = array_u32_push(i.value, vertices_carga.vecinos[j.value]);

    return vertices_carga;
}

static Vertice vertice_desde_array_u32(nombre_t nombre, color_t color, ArrayU32 orden_natural_de_vecinos) {
    u32 cantidad_de_vecinos = array_u32_length(orden_natural_de_vecinos);

    orden_natural_de_vecinos = array_u32_sort(orden_natural_de_vecinos);

    vecino_t *vecinos = (vecino_t *)safe_calloc(cantidad_de_vecinos, sizeof(vecino_t));

    for (u32 i = 0; i < cantidad_de_vecinos; i = i + 1) {
        vecinos[i] = vecino_of(
            orden_natural_of(array_u32_read_at(i, orden_natural_de_vecinos)),
            peso_of(0)
        );
    }

    array_u32_destroy(orden_natural_de_vecinos);

    return vertice_of(
        nombre,
        color,
        vecinos,
        cantidad_de_vecinos
    );
}

/** Pre: sorted_set_u32_size(nombres) == numero_de_vertices(vertices_carga) */
static Vertices vertices_carga_to_vertices(sorted_set_u32_borrowed nombres, vertices_carga_borrowed vertices_carga) {
    u32 numero_de_vertices = sorted_set_u32_size(nombres);

    Vertice *vertices_data = (Vertice *)safe_calloc(numero_de_vertices, sizeof(Vertice));

    for (u32 i = 0; true; i = i + 1) {
        maybe_nombre nombre = nombre_from_orden_natural(orden_natural_of(i), nombres);

        if (nombre.has_value) {
            vertices_data[i] = vertice_desde_array_u32(
                nombre.value,
                color_of(i),
                vertices_carga.vecinos[i]
            );
        } else {
            break;
        }
    }

    return vertices_of(
        vertices_data,
        numero_de_vertices
    );
}

static Vertices vertices_carga_to_vertices_b(sorted_set_u32_borrowed nombres, VerticesCarga vertices_carga) {
    Vertices result = vertices_carga_to_vertices(nombres, vertices_carga);
    vertices_carga_destroy(vertices_carga);
    return result;
}

// --- GRAFO DESDE DIMACS

MaybeGrafoSt grafo_desde_dimacs(dimacs_borrowed dimacs) {
    MaybeSortedSetU32 nombres = nombres_desde_dimacs(dimacs);

    if (!nombres.has_value) {
        return maybe_grafo_none();
    }

    VerticesCarga vertices_carga = vertices_carga_comenzar(dimacs.borrow.numero_de_vertices);

    for (u32 i = 0; true; i = i + 1) {
        maybe_pair_u32 lado = dimacs_lado(i, dimacs);

        if (!lado.has_value) {
            break;
        }

        maybe_orden_natural first = orden_natural_from_nombre(nombre_of(lado.value.first), nombres.value);
        maybe_orden_natural second = orden_natural_from_nombre(nombre_of(lado.value.second), nombres.value);

        if (!first.has_value || !second.has_value) {
            fprintf(stderr, "error: grafo_desde_dimacs !orden_natural.has_value\n");
            exit(-1);
        }

        vertices_carga = vertices_carga_agregar_lado(
            first.value,
            second.value,
            vertices_carga
        );
    }

    Vertices vertices = vertices_carga_to_vertices_b(nombres.value, vertices_carga);

    sorted_set_u32_destroy(nombres.value);

    return maybe_grafo_of(
        grafo_of(
            dimacs.borrow.numero_de_lados,
            vertices_get_delta(vertices),
            ordenamiento_trivial(vertices.numero_de_vertices),
            vertices
        )
    );
}

MaybeGrafoSt grafo_desde_dimacs_b(Dimacs dimacs) {
    MaybeGrafoSt result = grafo_desde_dimacs(dimacs_borrow(dimacs));
    dimacs_destroy(dimacs);
    return result;
}

#endif
