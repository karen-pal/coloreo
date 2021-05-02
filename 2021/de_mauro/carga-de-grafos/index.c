
#include "Grafo/grafo_desde_dimacs.c"
#include "RomaVictor.h"

/*

Este es el punto de entrada de la entrega. Basta con compilar este archivo para
que se compile todo el proyecto.

---

Muchas de las definiciones de este archivo son traducciones de llamadas de funciones de `Grafo/GrafoSt.c` para
adaptarlas a la API del proyecto.

(Decidí hacer definiciones por separado para poder usar más tipos y evitar errores tontos.)

*/

Grafo grafo_ptr_of(GrafoSt grafo) {
    Grafo grafo_ptr = (Grafo)safe_calloc(1, sizeof(GrafoSt));
    *grafo_ptr = grafo;
    return grafo_ptr;
}

/// 4.1
Grafo ConstruccionDelGrafo() {
    DimacsParseResult dimacs = dimacs_from_file(stdin);

    if (!dimacs.is_success) {
        dimacs_parse_result_destroy(dimacs);
        return NULL;
    }

    MaybeGrafoSt grafo = grafo_desde_dimacs_b(dimacs.success);

    if (!grafo.has_value) {
        return NULL;
    }

    return grafo_ptr_of(grafo.value);
}

/// 4.2
void DestruccionDelGrafo(Grafo grafo) {
    if (grafo != NULL) {
        grafo_destroy(*grafo);
        free(grafo);
    }
}

/// 4.3
Grafo CopiarGrafo(Grafo grafo) {
    if (grafo != NULL) {
        return grafo_ptr_of(grafo_copy(grafo_borrow(*grafo)));
    }
    return NULL;
}

/// 5.1
u32 NumeroDeVertices(Grafo grafo) {
    if (grafo == NULL) {
        return -1;
    }
    return grafo_numero_de_vertices(grafo_borrow(*grafo));
}

/// 5.2
u32 NumeroDeLados(Grafo grafo) {
    if (grafo == NULL) {
        return -1;
    }
    return grafo_numero_de_lados(grafo_borrow(*grafo));
}

/// 5.3
u32 Delta(Grafo grafo) {
    if (grafo == NULL) {
        return -1;
    }
    return grafo_delta(grafo_borrow(*grafo));
}

/// 6.1
u32 Nombre(u32 orden_interno, Grafo grafo) {
    if (grafo == NULL) {
        return -1;
    }

    return grafo_nombre_vertice(
        orden_interno_of(orden_interno),
        grafo_borrow(*grafo)
    ).value;
}

/// 6.2
u32 Color(u32 orden_interno, Grafo grafo) {
    if (grafo == NULL) {
        return -1;
    }

    return grafo_color_vertice(
        orden_interno_of(orden_interno),
        grafo_borrow(*grafo)
    ).value;
}

/// 6.3
u32 Grado(u32 orden_interno, Grafo grafo) {
    if (grafo == NULL) {
        return -1;
    }

    maybe_u32 grado = grafo_grado_vertice(
        orden_interno_of(orden_interno),
        grafo_borrow(*grafo)
    );

    if (grado.has_value) {
        return grado.value;
    } else {
        return -1;
    }
}

/// 7.1
u32 ColorVecino(u32 numero_de_vecino, u32 orden_interno, Grafo grafo) {
    if (grafo == NULL) {
        return -1;
    }

    return grafo_color_vecino(
        numero_de_vecino_of(numero_de_vecino),
        orden_interno_of(orden_interno),
        grafo_borrow(*grafo)
    ).value;
}

/// 7.2
u32 NombreVecino(u32 numero_de_vecino, u32 orden_interno, Grafo grafo) {
    if (grafo == NULL) {
        return -1;
    }

    return grafo_nombre_vecino(
        numero_de_vecino_of(numero_de_vecino),
        orden_interno_of(orden_interno),
        grafo_borrow(*grafo)
    ).value;
}

/// 7.3
u32 OrdenVecino(u32 numero_de_vecino, u32 orden_interno, Grafo grafo) {
    if (grafo == NULL) {
        return -1;
    }

    return grafo_orden_vecino(
        numero_de_vecino_of(numero_de_vecino),
        orden_interno_of(orden_interno),
        grafo_borrow(*grafo)
    ).value;
}

/// 7.4
u32 PesoLadoConVecino(u32 numero_de_vecino, u32 orden_interno, Grafo grafo) {
    if (grafo == NULL) {
        return -1;
    }

    return grafo_peso_lado(
        numero_de_vecino_of(numero_de_vecino),
        orden_interno_of(orden_interno),
        grafo_borrow(*grafo)
    ).value;
}

/// 8.1
char FijarColor(u32 color, u32 orden_interno, Grafo grafo) {
    if (grafo == NULL) {
        return result_to_char(FAILURE);
    }

    return result_to_char(
        grafo_fijar_color_vertice(
            color_of(color),
            orden_interno_of(orden_interno),
            grafo_mut_borrow(*grafo)
        )
    );
}

/// 8.2
char FijarOrden(u32 orden_interno, Grafo grafo, u32 orden_natural) {
    if (grafo == NULL) {
        return result_to_char(FAILURE);
    }

    return result_to_char(
        grafo_fijar_orden_vertice(
            orden_interno_of(orden_interno),
            orden_natural_of(orden_natural),
            grafo_mut_borrow(*grafo)
        )
    );
}

/// 8.3
u32 FijarPesoLadoConVecino(u32 numero_de_vecino, u32 orden_interno, u32 peso, Grafo grafo) {
    if (grafo == NULL) {
        return (u32)result_to_char(FAILURE);
    }

    return (u32)result_to_char(
        grafo_fijar_peso_lado(
            numero_de_vecino_of(numero_de_vecino),
            orden_interno_of(orden_interno),
            peso_of(peso),
            grafo_mut_borrow(*grafo)
        )
    );
}
