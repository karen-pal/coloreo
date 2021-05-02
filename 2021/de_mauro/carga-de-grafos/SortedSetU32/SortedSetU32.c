#ifndef SORTED_SET_U32
#define SORTED_SET_U32

#include "../ArrayU32/ArrayU32.c"
#include "../u32/maybe_u32.c"

typedef
    struct SortedSetU32 {
        ArrayU32 array;
    }
    SortedSetU32;

static void sorted_set_u32_destroy(SortedSetU32 sorted_set) {
    array_u32_destroy(sorted_set.array);
}

static SortedSetU32 sorted_set_u32_empty() {
    SortedSetU32 sorted_set;
    sorted_set.array = array_u32_empty();
    return sorted_set;
}

static SortedSetU32 sorted_set_u32_from_array(ArrayU32 array) {
    // Primero ordenamos el arreglo

    array = array_u32_sort(array);

    // Y ahora borramos los duplicados (que están contiguos).

    ArrayU32 unique = array_u32_empty();

    if (0 < array_u32_length(array)) {
        unique = array_u32_push(
            array_u32_read_at(0, array),
            unique
        );
    }

    for (u32 i = 1; i < array_u32_length(array); i = i + 1) {
        if (array_u32_read_at(i - 1, array) != array_u32_read_at(i, array)) {
            unique = array_u32_push(
                array_u32_read_at(i, array),
                unique
            );
        }
    }

    array_u32_destroy(array);

    SortedSetU32 sorted_set = { unique };
    return sorted_set;
}

u32 sorted_set_u32_size(SortedSetU32 sorted_set) {
    return array_u32_length(sorted_set.array);
}

typedef SortedSetU32 sorted_set_u32_borrowed;

static maybe_u32 sorted_set_u32_index_of_helper(u32 i, u32 j, u32 element, sorted_set_u32_borrowed sorted_set);

/** Busca el índice de `element` en O(log n). */
static maybe_u32 sorted_set_u32_index_of(u32 element, sorted_set_u32_borrowed sorted_set) {
    return sorted_set_u32_index_of_helper(0, sorted_set_u32_size(sorted_set), element, sorted_set);
}

/// Búsqueda binaria
static maybe_u32 sorted_set_u32_index_of_helper(u32 start, u32 end, u32 element, sorted_set_u32_borrowed sorted_set) {
    if (start == end) {
        if (element == array_u32_read_at(start, sorted_set.array)) {
            return maybe_u32_of(start);
        }
        return maybe_u32_none();
    }

    u32 i = (start + end) / 2;

    if (element == array_u32_read_at(i, sorted_set.array)) {
        return maybe_u32_of(i);
    }

    if (element > array_u32_read_at(i, sorted_set.array)) {
        return sorted_set_u32_index_of_helper(i, end, element, sorted_set);
    }

    return sorted_set_u32_index_of_helper(start, i, element, sorted_set);
}

static maybe_u32 sorted_set_u32_get_at_index(u32 index, sorted_set_u32_borrowed sorted_set) {
    if (index < array_u32_length(sorted_set.array)) {
        return maybe_u32_of(array_u32_read_at(index, sorted_set.array));
    }
    return maybe_u32_none();
}

typedef
    struct MaybeSortedSetU32 {
        bool has_value;
        SortedSetU32 value;
    }
    MaybeSortedSetU32;

static MaybeSortedSetU32 maybe_sorted_set_u32_of(SortedSetU32 ordenamiento) {
    MaybeSortedSetU32 maybe;
    maybe.has_value = true;
    maybe.value = ordenamiento;
    return maybe;
}

static MaybeSortedSetU32 maybe_sorted_set_u32_none() {
    MaybeSortedSetU32 maybe;
    maybe.has_value = false;
    return maybe;
}

#endif