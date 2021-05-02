#ifndef STRING
#define STRING

#include "../safe_stdlib.c"
#include <stdio.h>
#include <string.h>
#include "../common/common.c"
#include "../u32/maybe_u32.c"

struct String {
    CString data;
    u32 length;
    u32 capacity; // >= 8
};

typedef struct String String;

struct string_borrowed {
    /** Si tenemos una `string_borrowed`, tenemos prohibido alterar o destruir su `value`.
    */
    String borrow;
};

typedef struct string_borrowed string_borrowed;

/** Esta es la única función que toma un `String` y no lo destruye.
 * Las otras funciones que reciben `String`, siempre lo destruyen.
 * `string_borrow` se usa para indicar que una función no destruye ni modifica la string que recibe.
 * Para hacer la conversión inversa, o sea `string_borrowed -> String`, está `string_copy`
 * (no leer el `value` directamente).
*/
string_borrowed string_borrow(String string) {
    string_borrowed borrow = { string };
    return borrow;
}

static String string_of(CString source, u32 length, u32 capacity) {
    String string = { source, length, capacity };
    return string;
}


/*
La capacidad mínima de un string es 8. Esto es importante
porque nos evita lidiar con el hecho de que `calloc(0) == NULL`
(lo cual puede o no ser un problema), y también porque nos asegura
que `capacity = 2 * capacity` es siempre una forma correcta de aumentar la
capacidad.

Creo que el 90% de las veces que queremos una string vacía es para
después `append`arle cosas.
*/
String string_empty() {
    CString data = (CString)safe_calloc(8, sizeof(char));
    return string_of(data, 0, 8);
}

String string_from_memory(c_string source, u32 length) {
    if (source == NULL) {
        return string_empty();
    }
    u32 capacity = max(8, length);
    CString data = (CString)safe_calloc(capacity, sizeof(char));
    memcpy(data, source, length);

    return string_of(data, length, capacity);
}

/** Crea un string desde la memoria pero tomando solo hasta antes del primer 0.
 * El argumento `max_length` es una cota de tamaño y puede ser arbitrariamente grande.
*/
String string_from_c_string(c_string c_string, u32 max_length) {
    if (c_string == NULL) {
        return string_empty();
    }

    u32 length;

    for (
        length = 0;
        length < max_length && c_string[length] != '\0';
        length = length + 1
    );

    return string_from_memory(c_string, length);
}

void string_destroy(String string) {
    free(string.data);
}

String string_copy(string_borrowed string) {
    return string_from_memory(string.borrow.data, string.borrow.length);
}

bool string_starts_with(string_borrowed start, string_borrowed string) {
    return start.borrow.length <= string.borrow.length
        && memcmp(start.borrow.data, string.borrow.data, start.borrow.length) == 0;
}

bool string_starts_with_b(String start, string_borrowed string) {
    bool result = string_starts_with(string_borrow(start), string);
    string_destroy(start);
    return result;
}

bool string_ends_with(string_borrowed end, string_borrowed string) {
    return end.borrow.length <= string.borrow.length
        && memcmp(
            end.borrow.data,
            string.borrow.data + string.borrow.length - end.borrow.length,
            end.borrow.length
        ) == 0;
}

bool string_ends_with_b(String end, string_borrowed string) {
    bool result = string_ends_with(string_borrow(end), string);
    string_destroy(end);
    return result;
}

u32 string_length(string_borrowed string) {
    return string.borrow.length;
}

u32 string_length_b(String string) {
    u32 result = string_length(string_borrow(string));
    string_destroy(string);
    return result;
}

String string_substring(u32 from, u32 to, string_borrowed string) {
    if (from >= to
        || from >= string.borrow.length
    ) {
        return string_empty();
    }
    if (from == 0) {
        return string_from_memory(string.borrow.data, to);
    }

    return string_from_memory(
        string.borrow.data + from,
        min(string.borrow.length, to) - from
    );
}

String string_substring_b(u32 from, u32 to, String string) {
    String result = string_substring(from, to, string_borrow(string));
    string_destroy(string);
    return result;
}

/**
 * Se asegura que la capacidad de la string sea lo suficientemente grande como para
 * poder guardar `desired_length` elementos.
 */
static String string_with_capacity_for(u32 desired_length, String string) {
    if (desired_length <= string.capacity) {
        return string;
    }

    u32 new_capacity = string.capacity;

    while (new_capacity < desired_length) {
        new_capacity = 2 * new_capacity;
    }

    CString data = (CString)safe_realloc(string.data, new_capacity * sizeof(char));
    return string_of(data, string.length, new_capacity);
}

String string_append(string_borrowed appended, String string) {
    if (appended.borrow.length == 0) {
        return string;
    }

    string = string_with_capacity_for(string.length + appended.borrow.length, string);

    memcpy(string.data + string.length, appended.borrow.data, appended.borrow.length);

    string.length = string.length + appended.borrow.length;

    return string;
}

String string_append_b(String appended, String string) {
    String result = string_append(string_borrow(appended), string);
    string_destroy(appended);
    return result;
}

bool string_equals(string_borrowed a, string_borrowed b) {
    if (a.borrow.length != b.borrow.length) {
        return false;
    }
    if (a.borrow.length == 0) {
        return true;
    }

    return memcmp(a.borrow.data, b.borrow.data, a.borrow.length) == 0;
}

bool string_equals_b(String other, string_borrowed string) {
    bool equals = string_equals(string_borrow(other), string);
    string_destroy(other);
    return equals;
}

bool string_equals_c(String other, String string) {
    bool equals = string_equals_b(other, string_borrow(string));
    string_destroy(string);
    return equals;
}

int string_index_of(string_borrowed substring, string_borrowed string) {
    if (substring.borrow.length == string.borrow.length) {
        return string_equals(substring, string) ? 0 : -1;
    }

    for (int i = 0; i <= (int)string.borrow.length - (int)substring.borrow.length; i = i + 1) {
        bool match = memcmp(string.borrow.data + i, substring.borrow.data, substring.borrow.length) == 0;

        if (match) {
            return i;
        }
    }

    return -1;
}


// EXTRA

CString string_to_c_string(string_borrowed string) {
    CString c_string = (CString)safe_calloc(string.borrow.length + 1, sizeof(char));

    memcpy(c_string, string.borrow.data, string.borrow.length);

    return c_string;
}

/** Llama a fprintf */
void string_print(FILE *file, string_borrowed string) {

    char c_string[string.borrow.length + 1];

    memcpy(c_string, string.borrow.data, string.borrow.length);
    c_string[string.borrow.length] = '\0';

    fprintf(file, "%s", c_string);
}

void string_print_b(FILE *file, String string) {
    string_print(file, string_borrow(string));
    string_destroy(string);
}

void string_print_line(FILE *file, string_borrowed string) {
    String copy = string_copy(string);
    String newline = string_from_c_string("\n", 1);

    copy = string_append_b(newline, copy);
    string_print_b(file, copy);
}

maybe_u32 char_to_u32(char c) {
    switch (c) {
        case '0': return maybe_u32_of(0);
        case '1': return maybe_u32_of(1);
        case '2': return maybe_u32_of(2);
        case '3': return maybe_u32_of(3);
        case '4': return maybe_u32_of(4);
        case '5': return maybe_u32_of(5);
        case '6': return maybe_u32_of(6);
        case '7': return maybe_u32_of(7);
        case '8': return maybe_u32_of(8);
        case '9': return maybe_u32_of(9);
        default: return maybe_u32_none();
    }
}

maybe_u32 string_to_u32(string_borrowed string) {
    u32 result = 0;

    if (string.borrow.length == 0) {
        return maybe_u32_none();
    }

    for (u32 i = 0; i < string.borrow.length; i = i + 1) {
        maybe_u32 n = char_to_u32(string.borrow.data[i]);

        if (n.has_value) {
            result = result * 10 + n.value;
        } else {
            return maybe_u32_none();
        }
    }

    return maybe_u32_of(result);
}

String string_from_u32(u32 number) {
    char temp[256];
    sprintf(temp, "%u", number);
    return string_from_c_string(temp, 256);
}

#endif
