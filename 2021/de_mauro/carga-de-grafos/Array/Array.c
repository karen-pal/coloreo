#ifndef ARRAY
#define ARRAY

#include "../safe_stdlib.c"
#include <string.h>
#include "../common/common.c"

/* Invariante: capacity >= 8 */
struct Array {
    void *data;
    size_t element_size;
    u32 length;
    u32 capacity;
};

typedef struct Array Array;
typedef Array array_t;

static Array array_of(void *data_owned, size_t element_size, u32 length, u32 capacity) {
    Array array = { data_owned, element_size, length, capacity };
    return array;
}

Array array_from_memory(void *data_borrowed, size_t element_size, u32 length) {
    u32 capacity = max(8, length);

    void *data = safe_calloc(capacity, element_size);

    memcpy(data, data_borrowed, length * element_size);

    return array_of(data, element_size, length, capacity);
}

Array array_copy(array_t array) {
    return array_from_memory(
        array.data,
        array.element_size,
        array.length
    );
}

typedef void destroy_fn(void *a);

void *array_read_at(u32 index, array_t array);

void array_destroy(destroy_fn destroy_element, Array array) {
    for (u32 i = 0; i < array.length; i = i + 1) {
        destroy_element(array_read_at(i, array));
    }

    free(array.data);
}

Array array_empty(size_t element_size) {
    void *data = safe_calloc(8, element_size);

    return array_of(
        data,
        element_size,
        0,
        8
    );
}

/** Crea un arreglo con `length` elementos inicializados en 0. */
Array array_blank(u32 length, size_t element_size) {
    u32 capacity = max(8, length);

    return array_of(
        safe_calloc(capacity, element_size),
        element_size,
        length,
        capacity
    );
}

static Array array_duplicate_capacity(Array array) {
    u32 capacity = array.capacity * 2;

    return array_of(
        safe_realloc(array.data, capacity * array.element_size),
        array.element_size,
        array.length,
        capacity
    );
}

Array array_write_at(void *element, u32 index, Array array) {
    if (index >= array.capacity) {
        return array;
    }
    memcpy(
        array.data + index * array.element_size,
        element,
        array.element_size
    );
    return array;
}

/** Devuelve NULL si el índice está fuera de rango. */
void *array_read_at(u32 index, array_t array) {
    if (index < array.length) {
        return array.data + index * array.element_size;
    } else {
        return NULL;
    }
}

u32 array_length(array_t array) {
    return array.length;
}

Array array_push(void *element, Array array) {
    if (array.length < array.capacity) {
        array_write_at(element, array.length, array);
        return array_of(array.data, array.element_size, array.length + 1, array.capacity);
    } else {
        return array_push(element, array_duplicate_capacity(array));
    }
}

Array array_swap(u32 i, u32 j, Array array) {
    u32 temp[array.element_size / sizeof(u32) + 1];

    memcpy(temp, array_read_at(i, array), array.element_size);

    array_write_at(
        array_read_at(j, array),
        i,
        array
    );

    array_write_at(
        temp,
        j,
        array
    );

    return array;
}

Array array_reverse(Array array) {
    for (u32 index = 0; index < array.length / 2; index = index + 1) {
        array_swap(index, array.length - 1 - index, array);
    }
    return array;
}

/** Subarreglo entre `from` (inclusivo) hasta `to` (exclusivo) */
Array array_subarray(u32 from, u32 to, array_t array) {
    if (from >= to
        || from >= array.length
    ) {
        return array_empty(array.capacity);
    }
    if (from == 0) {
        return array_from_memory(array.data, array.element_size, to);
    }
    return array_from_memory(
        array.data + from,
        array.element_size,
        min(array.length, to) - from
    );
}

typedef bool equals_fn(void *a, void *b);

bool array_equals(equals_fn element_equals, array_t a, array_t b) {
    if (a.element_size != b.element_size || a.length != b.length) {
        return false;
    }

    for (u32 i = 0; i < a.length; i = i + 1) {
        if (!element_equals(array_read_at(i, a), array_read_at(i, b))) {
            return false;
        }
    }

    return true;
}

typedef int compare_fn(const void *a, const void *b);

Array array_sort(compare_fn compare_element, Array array) {
    qsort(array.data, array.length, array.element_size, compare_element);
    return array;
}

#endif
