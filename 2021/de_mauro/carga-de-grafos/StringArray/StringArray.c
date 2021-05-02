#ifndef STRING_ARRAY
#define STRING_ARRAY

#include "../Array/Array.c"
#include "../String/String.c"

struct StringArray {
    Array string_array;
};

typedef struct StringArray StringArray;
typedef struct StringArray string_array;

static StringArray string_array_of(Array array) {
    StringArray string_array = { array };
    return string_array;
}

StringArray string_array_from_memory(String *data_borrowed, u32 length) {
    return string_array_of(array_from_memory(data_borrowed, sizeof(String), length));
}

StringArray string_array_copy(string_array array) {
    return string_array_of(array_copy(array.string_array));
}

static void string_destroy_(void *string) {
    string_destroy(*(String *)string);
}

void string_array_destroy(StringArray array) {
    array_destroy(string_destroy_, array.string_array);
}

StringArray string_array_empty() {
    return string_array_of(array_empty(sizeof(String)));
}

StringArray string_array_write_at(String element, u32 index, StringArray array) {
    return string_array_of(array_write_at(&element, index, array.string_array));
}

/** Pre: index < string_array_length(array).
 * Las lecturas fuera de rango provocan comportamientos inesperados.
 */
string_borrowed string_array_read_at(u32 index, string_array array) {
    String *string = array_read_at(index, array.string_array);

    if (string == NULL) {
        fprintf(stderr, "Invalid string_array_read_at call\n");
        exit(-1);
    }

    return string_borrow(*string);
}

u32 string_array_length(string_array array) {
    return array_length(array.string_array);
}

StringArray string_array_push(String element, StringArray array) {
    return string_array_of(array_push(&element, array.string_array));
}

StringArray string_array_singleton(String string) {
    StringArray array = string_array_empty();
    return string_array_push(string, array);
}

StringArray string_array_subarray(u32 from, u32 to, string_array array) {
    return string_array_of(array_subarray(from, to, array.string_array));
}

static bool string_equals_(void *a, void *b) {
    return string_equals(string_borrow(*(String *)a), string_borrow(*(String *)b));
}

bool string_array_equals(StringArray a, StringArray b) {
    return array_equals(string_equals_, a.string_array, b.string_array);
}

StringArray string_array_reverse(StringArray array) {
    return string_array_of(array_reverse(array.string_array));
}

static StringArray string_split_reversed(string_borrowed marker, string_borrowed string) {
    int index_of_marker = string_index_of(marker, string);

    if (index_of_marker == -1) {
        return string_array_singleton(string_copy(string));
    }

    String head = string_substring(0, index_of_marker, string);
    String tail = string_substring(
        index_of_marker + string_length(marker),
        string_length(string),
        string
    );

    StringArray split_tail = string_split_reversed(marker, string_borrow(tail));

    string_destroy(tail);

    return string_array_push(head, split_tail);
}

StringArray string_split(string_borrowed marker, string_borrowed string) {
    return string_array_reverse(string_split_reversed(marker, string));
}

StringArray string_split_b(String marker, string_borrowed string) {
    StringArray result = string_split(string_borrow(marker), string);
    string_destroy(marker);
    return result;
}

StringArray string_split_c(String marker, String string) {
    StringArray result = string_split_b(marker, string_borrow(string));
    string_destroy(string);
    return result;
}

String string_array_join(string_borrowed join, string_array array) {
    String result = string_empty();

    for (
        u32 i = 0;
        i < array.string_array.length - 1;
        i = i + 1
    ) {
        result = string_append(string_array_read_at(i, array), result);
        result = string_append(join, result);
    }

    if (array.string_array.length > 0) {
        result = string_append(
            string_array_read_at(array.string_array.length - 1, array),
            result
        );
    }

    return result;
}

String string_array_join_b(String join, string_array array) {
    String result = string_array_join(string_borrow(join), array);
    string_destroy(join);
    return result;
}

StringArray string_array_from_main_args(int argc, char *argv[]) {
    StringArray array = string_array_empty();

    for (int i = 0; i < argc; i = i + 1) {
        array = string_array_push(string_from_c_string(argv[i], 256), array);
    }

    return array;
}

#endif
