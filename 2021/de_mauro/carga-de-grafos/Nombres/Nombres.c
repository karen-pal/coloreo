#ifndef NOMBRES
#define NOMBRES

#include "../Dimacs/Dimacs.c"

// --- NOMBRES

// Le llamo "nombres" a un SortedSetU32 obtenido a partir de los lados de un dimacs.
// Tiene la propiedad de que nos permite hacer la conversión `nombre_t -> orden_natural_t`
// en un tiempo razonable.

/// Devuelve los nombres del grafo como un SortedSetU32.
/// Puede devolver none si el header del Dimacs tiene mal declarado el número de vértices.
MaybeSortedSetU32 nombres_desde_dimacs(dimacs_borrowed dimacs) {
    SortedSetU32 sorted_set = sorted_set_u32_from_array(
        array_u32_from_memory((u32 *)dimacs.borrow.lados, dimacs.borrow.numero_de_lados * 2)
    );

    if (sorted_set_u32_size(sorted_set) != dimacs.borrow.numero_de_vertices) {
        sorted_set_u32_destroy(sorted_set);
        return maybe_sorted_set_u32_none();
    }

    return maybe_sorted_set_u32_of(sorted_set);
}

/// O(log n)
maybe_orden_natural orden_natural_from_nombre(nombre_t nombre, sorted_set_u32_borrowed nombres) {
    maybe_u32 maybe = sorted_set_u32_index_of(nombre.value, nombres);

    if (maybe.has_value) {
        return maybe_orden_natural_of(orden_natural_of(maybe.value));
    } else {
        return maybe_orden_natural_none();
    }
}

maybe_nombre nombre_from_orden_natural(orden_natural_t orden_natural, sorted_set_u32_borrowed nombres) {
    maybe_u32 maybe = sorted_set_u32_get_at_index(orden_natural.value, nombres);

    if (maybe.has_value) {
        return maybe_nombre_of(nombre_of(maybe.value));
    }
    return maybe_nombre_none();
}

#endif
