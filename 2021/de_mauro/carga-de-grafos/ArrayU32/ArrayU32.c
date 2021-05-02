#ifndef ARRAY_U32
#define ARRAY_U32

#include <stdio.h>
#include "../Array/Array.c"


struct ArrayU32 {
    Array array_u32;
};

typedef struct ArrayU32 ArrayU32;
typedef struct ArrayU32 array_u32_borrowed;

static ArrayU32 array_u32_of(Array array) {
    ArrayU32 array_u32 = { array };
    return array_u32;
}

ArrayU32 array_u32_from_memory(u32 *data_borrowed, u32 length) {
    return array_u32_of(array_from_memory(data_borrowed, sizeof(u32), length));
}

ArrayU32 array_u32_from_memory_b(u32 *data_owned, u32 length) {
    ArrayU32 result = array_u32_from_memory(data_owned, length);
    free(data_owned);
    return result;
}

ArrayU32 array_u32_copy(array_u32_borrowed array) {
    return array_u32_of(array_copy(array.array_u32));
}

#define UNUSED(x) (void)(x)
static void u32_destroy_(void *u32) { UNUSED(u32); }

void array_u32_destroy(ArrayU32 array) {
    array_destroy(u32_destroy_, array.array_u32);
}

ArrayU32 array_u32_empty() {
    return array_u32_of(array_empty(sizeof(u32)));
}

/** Devuelve el contenido del arreglo en un bloque de memoria de tamaño `array_length(array)`. */
u32 *array_u32_to_memory(ArrayU32 array) {
    if (array.array_u32.length == array.array_u32.capacity) {
        return (u32 *)array.array_u32.data;
    }

    u32 *memory = (u32 *)safe_calloc(array.array_u32.length, sizeof(u32));
    memcpy(memory, array.array_u32.data, array.array_u32.length * sizeof(u32));

    array_u32_destroy(array);

    return memory;
}

ArrayU32 array_u32_write_at(u32 element, u32 index, ArrayU32 array) {
    return array_u32_of(array_write_at(&element, index, array.array_u32));
}

/** Pre: index < array_u32_length(array).
 * Las lecturas fuera de rango provocan crash.
 */
u32 array_u32_read_at(u32 index, array_u32_borrowed array) {
    u32 *u32_ptr = array_read_at(index, array.array_u32);

    if (u32_ptr == NULL) {
        fprintf(stderr, "error: array_u32_read_at NULL\n");
        exit(-1);
    }

    return *u32_ptr;
}

u32 array_u32_length(array_u32_borrowed array) {
    return array_length(array.array_u32);
}

ArrayU32 array_u32_push(u32 element, ArrayU32 array) {
    return array_u32_of(array_push(&element, array.array_u32));
}

ArrayU32 array_u32_singleton(u32 string) {
    ArrayU32 array = array_u32_empty();
    return array_u32_push(string, array);
}

ArrayU32 array_u32_subarray(u32 from, u32 to, array_u32_borrowed array) {
    return array_u32_of(array_subarray(from, to, array.array_u32));
}

static bool u32_equals(void *a, void *b) {
    return *(u32 *)a == *(u32 *)b;
}

bool array_u32_equals(ArrayU32 a, ArrayU32 b) {
    return array_equals(u32_equals, a.array_u32, b.array_u32);
}

ArrayU32 array_u32_reverse(ArrayU32 array) {
    return array_u32_of(array_reverse(array.array_u32));
}

int compare_u32(const void *a, const void *b) {
    if (*(u32 *)a < *(u32 *)b) {
        return -1;
    } else if (*(u32 *)a == *(u32 *)b) {
        return 0;
    } else {
        return 1;
    }
}

ArrayU32 array_u32_sort(ArrayU32 array) {
    return array_u32_of(array_sort(compare_u32, array.array_u32));
}

/// Devuelve un arreglo [start, start+1, ..., end-1, end]
ArrayU32 array_u32_range(u32 start, u32 end) {
    ArrayU32 array = array_u32_empty();

    for (
        u32 i = start;
        i <= end;
        i = i + 1
    ) {
        array = array_u32_push(i, array);
    }

    return array;
}

#endif
