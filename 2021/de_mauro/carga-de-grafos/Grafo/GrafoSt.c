#ifndef GRAFO
#define GRAFO

#include "../u32/maybe_u32.c"
#include "../Vertices/Vertices.c"
#include "../Ordenamiento/Ordenamiento.c"
#include "../RomaVictor.h"

// --- GrafoSt

/*
 * Ver la estructura definida en GrafoSt21.h, que se incluye
 * indirectamente a través de los includes de este archivo.
*/

GrafoSt grafo_of(u32 numero_de_lados, u32 delta, Ordenamiento ordenamiento, Vertices vertices) {
    GrafoSt grafo;
    grafo.numero_de_lados = numero_de_lados;
    grafo.delta = delta;
    grafo.ordenamiento = ordenamiento;
    grafo.vertices = vertices;
    return grafo;
}


typedef
    struct grafo_borrowed {
        GrafoSt borrow;
    }
    grafo_borrowed;

grafo_borrowed grafo_borrow(GrafoSt grafo) {
    grafo_borrowed borrow = { grafo };
    return borrow;
}

typedef
    struct grafo_mut_borrowed {
        GrafoSt mut_borrow;
    }
    grafo_mut_borrowed;

grafo_mut_borrowed grafo_mut_borrow(GrafoSt grafo) {
    grafo_mut_borrowed mut_borrow = { grafo };
    return mut_borrow;
}

typedef
    struct MaybeGrafoSt {
        bool has_value;
        GrafoSt value;
    }
    MaybeGrafoSt;

MaybeGrafoSt maybe_grafo_none() {
    MaybeGrafoSt maybe = { false };
    return maybe;
}

MaybeGrafoSt maybe_grafo_of(GrafoSt grafo) {
    MaybeGrafoSt maybe;
    maybe.has_value = true;
    maybe.value = grafo;
    return maybe;
}

void grafo_destroy(GrafoSt grafo) {
    ordenamiento_destroy(grafo.ordenamiento);
    vertices_destroy(grafo.vertices);
}

GrafoSt grafo_copy(grafo_borrowed grafo) {
    return grafo_of(
        grafo.borrow.numero_de_lados,
        grafo.borrow.delta,
        ordenamiento_copy(grafo.borrow.ordenamiento),
        vertices_copy(grafo.borrow.vertices)
    );
}

// ---

u32 grafo_numero_de_vertices(grafo_borrowed grafo) {
    return grafo.borrow.vertices.numero_de_vertices;
}

u32 grafo_numero_de_lados(grafo_borrowed grafo) {
    return grafo.borrow.numero_de_lados;
}

u32 grafo_delta(grafo_borrowed grafo) {
    return grafo.borrow.delta;
}

// --- Propiedades de vértices

/// 6.1
nombre_t grafo_nombre_vertice(orden_interno_t i, grafo_borrowed grafo) {
    maybe_orden_natural orden_natural = orden_natural_from_orden_interno(
        i,
        grafo.borrow.ordenamiento
    );

    if (!orden_natural.has_value) {
        return nombre_of(-1);
    }

    maybe_borrowed_vertice vertice = vertices_leer(
        orden_natural.value,
        grafo.borrow.vertices
    );

    if (vertice.has_value) {
        return vertice.value.nombre;
    } else {
        return nombre_of(-1);
    }
}

/// 6.2
color_t grafo_color_vertice(orden_interno_t i, grafo_borrowed grafo) {
    maybe_orden_natural orden_natural = orden_natural_from_orden_interno(
        i,
        grafo.borrow.ordenamiento
    );

    if (!orden_natural.has_value) {
        return color_of(-1);
    }

    maybe_borrowed_vertice vertice = vertices_leer(
        orden_natural.value,
        grafo.borrow.vertices
    );

    if (vertice.has_value) {
        return vertice.value.color;
    } else {
        return color_of(-1);
    }
}

/// 6.3
maybe_u32 grafo_grado_vertice(orden_interno_t i, grafo_borrowed grafo) {
    maybe_orden_natural orden_natural = orden_natural_from_orden_interno(
        i,
        grafo.borrow.ordenamiento
    );

    if (!orden_natural.has_value) {
        return maybe_u32_none();
    }

    maybe_borrowed_vertice vertice = vertices_leer(
        orden_natural.value,
        grafo.borrow.vertices
    );

    if (vertice.has_value) {
        return maybe_u32_of(vertice.value.cantidad_de_vecinos);
    } else {
        return maybe_u32_none();
    }
}

// --- Propiedades de vecinos

static maybe_vecino_t grafo_leer_vecino_desde_indices(numero_de_vecino_t j, orden_interno_t i, grafo_borrowed grafo) {
    maybe_orden_natural orden_natural = orden_natural_from_orden_interno(
        i,
        grafo.borrow.ordenamiento
    );

    if (!orden_natural.has_value) {
        return maybe_vecino_none();
    }

    maybe_borrowed_vertice vertice = vertices_leer(
        orden_natural.value,
        grafo.borrow.vertices
    );

    if (!vertice.has_value) {
        return maybe_vecino_none();
    }

    return vertices_leer_vecino(
        j,
        vertice.value,
        grafo.borrow.vertices
    );
}

static maybe_borrowed_vertice grafo_leer_vertice_desde_indices(numero_de_vecino_t j, orden_interno_t i, grafo_borrowed grafo) {
    maybe_vecino_t vecino = grafo_leer_vecino_desde_indices(j, i, grafo);

    if (!vecino.has_value) {
        return maybe_borrowed_vertice_none();
    }

    maybe_borrowed_vertice vertice = vertices_leer(
        vecino.value.orden_natural,
        grafo.borrow.vertices
    );

    if (vertice.has_value) {
        return maybe_borrowed_vertice_of(vertice.value);
    } else {
        return maybe_borrowed_vertice_none();
    }
}

/// 7.1
color_t grafo_color_vecino(numero_de_vecino_t j, orden_interno_t i, grafo_borrowed G) {
    maybe_borrowed_vertice vertice = grafo_leer_vertice_desde_indices(j, i, G);

    if (vertice.has_value) {
        return vertice.value.color;
    } else {
        return color_of(-1);
    }
}

/// 7.2
nombre_t grafo_nombre_vecino(numero_de_vecino_t j, orden_interno_t i, grafo_borrowed G) {
    maybe_borrowed_vertice vertice = grafo_leer_vertice_desde_indices(j, i, G);

    if (vertice.has_value) {
        return vertice.value.nombre;
    } else {
        return nombre_of(-1);
    }
}

/// 7.3
orden_interno_t grafo_orden_vecino(numero_de_vecino_t j, orden_interno_t i, grafo_borrowed G) {
    maybe_vecino_t vecino = grafo_leer_vecino_desde_indices(j, i, G);

    if (vecino.has_value) {
        maybe_orden_interno orden_interno = orden_interno_from_orden_natural(
            vecino.value.orden_natural,
            G.borrow.ordenamiento
        );

        if (orden_interno.has_value) {
            return orden_interno.value;
        }
    }

    return orden_interno_of(-1);
}

/// 7.4
peso_t grafo_peso_lado(numero_de_vecino_t j, orden_interno_t i, grafo_borrowed G) {
    maybe_vecino_t vecino = grafo_leer_vecino_desde_indices(j, i, G);

    if (vecino.has_value) {
        return vecino.value.peso;
    } else {
        return peso_of(-1);
    }
}

/// 8.1
result_t grafo_fijar_color_vertice(color_t x, orden_interno_t i, grafo_mut_borrowed grafo) {
    maybe_orden_natural orden_natural = orden_natural_from_orden_interno(
        i,
        grafo.mut_borrow.ordenamiento
    );

    if (!orden_natural.has_value) {
        return FAILURE;
    }

    return vertices_fijar_color(
        x,
        orden_natural.value,
        grafo.mut_borrow.vertices
    );
}

/// 8.2
result_t grafo_fijar_orden_vertice(orden_interno_t x, orden_natural_t N, grafo_mut_borrowed grafo) {
    return fijar_orden_interno(x, N, grafo.mut_borrow.ordenamiento);
}

/// Esta función es para testear. Devuelve orden_interno_of(-1) si hay error.
orden_interno_t grafo_orden_vertice(orden_natural_t n, grafo_borrowed grafo) {
    maybe_orden_interno orden_interno = orden_interno_from_orden_natural(n, grafo.borrow.ordenamiento);

    if (orden_interno.has_value) {
        return orden_interno.value;
    }
    return orden_interno_of(-1);
}

result_t grafo_fijar_vecino(vecino_t vecino, numero_de_vecino_t j, orden_interno_t i, grafo_mut_borrowed grafo) {
    maybe_orden_natural orden_natural = orden_natural_from_orden_interno(
        i,
        grafo.mut_borrow.ordenamiento
    );

    if (!orden_natural.has_value) {
        return FAILURE;
    }

    maybe_borrowed_vertice vertice = vertices_leer(
        orden_natural.value,
        grafo.mut_borrow.vertices
    );

    if (vertice.has_value) {
        return vertice_escribir_vecino(vecino, j, vertice.value);
    }

    return FAILURE;
}

/// 8.3
result_t grafo_fijar_peso_lado(numero_de_vecino_t j, orden_interno_t i, peso_t p, grafo_mut_borrowed grafo) {
    maybe_vecino_t vecino = grafo_leer_vecino_desde_indices(j, i, grafo_borrow(grafo.mut_borrow));

    if (!vecino.has_value) {
        return FAILURE;
    }

    vecino.value.peso = p;

    return grafo_fijar_vecino(vecino.value, j, i, grafo);
}

#endif
