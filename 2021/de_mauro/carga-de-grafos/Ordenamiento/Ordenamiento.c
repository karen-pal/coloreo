#ifndef ORDENAMIENTO
#define ORDENAMIENTO

#include "../Tipos/Tipos.c"
#include "../safe_stdlib.c"
#include "Ordenamiento.h"

// --- Ordenamiento

typedef Ordenamiento ordenamiento_borrowed;

static Ordenamiento ordenamiento_of(
    u32 numero_de_vertices,
    orden_natural_t *orden_natural,
    orden_interno_t *orden_interno
) {
    Ordenamiento ordenamiento;
    ordenamiento.numero_de_vertices = numero_de_vertices;
    ordenamiento.orden_natural = orden_natural;
    ordenamiento.orden_interno = orden_interno;
    return ordenamiento;
}

void ordenamiento_destroy(Ordenamiento ordenamiento) {
    free(ordenamiento.orden_natural);
    free(ordenamiento.orden_interno);
}

Ordenamiento ordenamiento_copy(ordenamiento_borrowed ordenamiento) {
    return ordenamiento_of(
        ordenamiento.numero_de_vertices,
        (orden_natural_t *)memcpy(
            safe_calloc(ordenamiento.numero_de_vertices, sizeof(orden_natural_t)),
            ordenamiento.orden_natural,
            ordenamiento.numero_de_vertices * sizeof(orden_natural_t)
        ),
        (orden_interno_t *)memcpy(
            safe_calloc(ordenamiento.numero_de_vertices, sizeof(orden_interno_t)),
            ordenamiento.orden_natural,
            ordenamiento.numero_de_vertices * sizeof(orden_interno_t)
        )
    );
}

/** Modifica pero NO destruye `ordenamiento`. */
result_t fijar_orden_interno(orden_interno_t orden_interno, orden_natural_t orden_natural, Ordenamiento ordenamiento) {
    if (ordenamiento.orden_interno != 0
        && ordenamiento.orden_natural != 0
        && orden_interno.value < ordenamiento.numero_de_vertices
        && orden_natural.value < ordenamiento.numero_de_vertices
    ) {
        ordenamiento.orden_interno[orden_natural.value] = orden_interno;
        ordenamiento.orden_natural[orden_interno.value] = orden_natural;
        return SUCCESS;
    }
    return FAILURE;
}

maybe_orden_natural orden_natural_from_orden_interno(orden_interno_t orden_interno, ordenamiento_borrowed ordenamiento) {
    if (ordenamiento.orden_natural == 0
        || orden_interno.value >= ordenamiento.numero_de_vertices
    ) {
        return maybe_orden_natural_none();
    }
    return maybe_orden_natural_of(ordenamiento.orden_natural[orden_interno.value]);
}

maybe_orden_interno orden_interno_from_orden_natural(orden_natural_t orden_natural, ordenamiento_borrowed ordenamiento) {
    if (ordenamiento.orden_interno == 0
        || orden_natural.value >= ordenamiento.numero_de_vertices
    ) {
        return maybe_orden_interno_none();
    }
    return maybe_orden_interno_of(ordenamiento.orden_interno[orden_natural.value]);
}

Ordenamiento ordenamiento_trivial(u32 numero_de_vertices) {
    Ordenamiento ordenamiento = ordenamiento_of(
        numero_de_vertices,
        (orden_natural_t *)safe_calloc(numero_de_vertices, sizeof(orden_natural_t)),
        (orden_interno_t *)safe_calloc(numero_de_vertices, sizeof(orden_interno_t))
    );

    for (u32 i = 0; true; i = i + 1) {
        result_t result = fijar_orden_interno(
            orden_interno_of(i),
            orden_natural_of(i),
            ordenamiento
        );

        if (result == SUCCESS) {
            continue;
        } else {
            break;
        }
    }

    return ordenamiento;
}

#endif
