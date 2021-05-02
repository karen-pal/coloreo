#ifndef VERTICES
#define VERTICES

#include "../Tipos/Tipos.c"
#include "../safe_stdlib.c"
#include "Vertices.h"

// --- Vecinos y vértices

vecino_t vecino_of(orden_natural_t orden_natural, peso_t peso) {
    vecino_t vecino;
    vecino.orden_natural = orden_natural;
    vecino.peso = peso;
    return vecino;
}

struct maybe_vecino_t {
    bool has_value;
    vecino_t value;
};

typedef struct maybe_vecino_t maybe_vecino_t;

maybe_vecino_t maybe_vecino_of(vecino_t vecino) {
    maybe_vecino_t maybe = { true, vecino };
    return maybe;
}

maybe_vecino_t maybe_vecino_none() {
    maybe_vecino_t maybe = { false };
    return maybe;
}


Vertice vertice_of(nombre_t nombre, color_t color, vecino_t *vecinos, u32 cantidad_de_vecinos) {
    Vertice vertice;
    vertice.nombre = nombre;
    vertice.color = color;
    vertice.vecinos = vecinos;
    vertice.cantidad_de_vecinos = cantidad_de_vecinos;
    return vertice;
}

void vertice_destroy(Vertice vertice) {
    free(vertice.vecinos);
}

typedef Vertice vertice_borrowed;
typedef Vertice vertice_mut_borrowed;

Vertice vertice_copy(vertice_borrowed vertice) {
    return vertice_of(
        vertice.nombre,
        vertice.color,
        (vecino_t *)memcpy(
            safe_calloc(vertice.cantidad_de_vecinos, sizeof(vecino_t)),
            vertice.vecinos,
            vertice.cantidad_de_vecinos * sizeof(vecino_t)
        ),
        vertice.cantidad_de_vecinos
    );
}

static Vertices vertices_of(Vertice *vertices, u32 numero_de_vertices) {
    Vertices vertices_;
    vertices_.vertices = vertices;
    vertices_.numero_de_vertices = numero_de_vertices;
    return vertices_;
}

void vertices_destroy(Vertices vertices) {
    for (u32 i = 0; i < vertices.numero_de_vertices; i++) {
        vertice_destroy(vertices.vertices[i]);
    }
    free(vertices.vertices);
}

/* Tener un `vertices_borrowed` significa que no somos el dueño, o sea no estamos encargados de modificar ni destruír nada.
Tener un `Vertices`, por el contrario, significa que debemos sí o sí destruirlo. Dar un `Vertices` como argumento a una función
también cuenta como "destruirlo", ya que si la función lo recibe como `Vertices` significa que ahora ella lo va a destruír.
La única excepción es la función `vertices_borrow`, que no destruye a los `Vertices` que recibe.
 */
typedef Vertices vertices_borrowed;

typedef Vertices vertices_mut_borrowed;

Vertices vertices_copy(vertices_borrowed vertices) {
    Vertice *vertices_data = (Vertice *)safe_calloc(vertices.numero_de_vertices, sizeof(Vertice));

    for (u32 i = 0; i < vertices.numero_de_vertices; i = i + 1) {
        vertices_data[i] = vertice_copy(vertices.vertices[i]);
    }

    return vertices_of(vertices_data, vertices.numero_de_vertices);
}

struct maybe_borrowed_vertice {
    bool has_value;
    vertice_borrowed value;
};

typedef struct maybe_borrowed_vertice maybe_borrowed_vertice;

maybe_borrowed_vertice maybe_borrowed_vertice_of(vertice_borrowed vertice) {
    maybe_borrowed_vertice maybe;
    maybe.has_value = true;
    maybe.value = vertice;
    return maybe;
}

maybe_borrowed_vertice maybe_borrowed_vertice_none() {
    maybe_borrowed_vertice maybe;
    maybe.has_value = false;
    return maybe;
}

maybe_borrowed_vertice vertices_leer(orden_natural_t orden_natural, vertices_borrowed vertices) {
    if (vertices.vertices == 0
        || orden_natural.value >= vertices.numero_de_vertices
    ) {
        return maybe_borrowed_vertice_none();
    }
    return maybe_borrowed_vertice_of(vertices.vertices[orden_natural.value]);
}

result_t vertices_fijar_color(
    color_t color,
    orden_natural_t orden_natural,
    vertices_mut_borrowed vertices
) {
    if (vertices.vertices == 0
        || orden_natural.value >= vertices.numero_de_vertices
    ) {
        return FAILURE;
    }
    Vertice vertice = vertices.vertices[orden_natural.value];

    vertices.vertices[orden_natural.value] = vertice_of(
        vertice.nombre,
        color,
        vertice.vecinos,
        vertice.cantidad_de_vecinos
    );

    return SUCCESS;
}

maybe_vecino_t vertices_leer_vecino(numero_de_vecino_t numero_de_vecino, vertice_borrowed vertice, vertices_borrowed vertices) {
    if (vertices.vertices == 0
        || vertice.vecinos == 0
        || numero_de_vecino.value >= vertice.cantidad_de_vecinos
    ) {
        return maybe_vecino_none();
    }
    return maybe_vecino_of(vertice.vecinos[numero_de_vecino.value]);
}

/** Muta pero no destruye `vertice`. */
result_t vertice_escribir_vecino(
    vecino_t vecino,
    numero_de_vecino_t numero_de_vecino,
    vertice_mut_borrowed vertice
) {
    if (vertice.vecinos == 0
        || numero_de_vecino.value >= vertice.cantidad_de_vecinos
    ) {
        return FAILURE;
    }
    vertice.vecinos[numero_de_vecino.value] = vecino;
    return SUCCESS;
}

u32 vertices_get_delta(vertices_borrowed vertices) {
    u32 delta = 0;

    for (u32 i = 0; true; i = i + 1) {
        maybe_borrowed_vertice vertice = vertices_leer(orden_natural_of(i), vertices);

        if (vertice.has_value) {
            delta = max(delta, vertice.value.cantidad_de_vecinos);
        } else {
            break;
        }
    }

    return delta;
}

#endif
