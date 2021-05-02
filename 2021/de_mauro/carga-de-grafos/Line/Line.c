#include <stdio.h>
#include <string.h>

#include "../String/String.c"
#include "../String/MaybeString.c"

/**
 * Una línea de un archivo de texto.
 * Contiene una `String` si no es la última linea o el EOF.
 * La `String` no incluye el `\n` ni el `\r`.
 */
struct Line {
    MaybeString string;
    u32 number;
};

typedef struct Line Line;

Line line_of(String string, u32 number) {
    Line line;
    line.string = maybe_string_of(string);
    line.number = number;
    return line;
}

Line line_eof(u32 line_number) {
    Line line;
    line.string = maybe_string_none();
    line.number = line_number;
    return line;
}

/**
 * Podría definir esto como:
 *
 *     struct line_borrowed { Line borrow };
 *
 * Pero era más ergonómico de usar si lo definía así:
*/
typedef
    struct line_borrowed {
        maybe_string_borrowed string;
        u32 number;
    }
    line_borrowed;

line_borrowed line_borrow(Line line) {
    line_borrowed borrow;
    borrow.string = maybe_string_borrow(line.string);
    borrow.number = line.number;
    return borrow;
}

Line line_copy(line_borrowed line) {
    if (line.string.has_value) {
        String string = string_copy(line.string.value);
        return line_of(string, line.number);
    } else {
        return line_eof(line.number);
    }
}

void line_destroy(Line line) {
    maybe_string_destroy(line.string);
}

static MaybeString c_line_from_file(FILE *file_mut_borrow, u32 max_read_length) {
    String string = string_empty();

    for (
        u32 i = 0;
        i < max_read_length;
        i = i + 1
    ) {
        int fgetc_response = fgetc(file_mut_borrow);

        if (fgetc_response == EOF) {
            if (i == 0) {
                // Encontrar un EOF como primer caracter en una línea
                // significa que el archivo ya terminó.
                string_destroy(string);
                return maybe_string_none();
            } else {
                // Encontrar un EOF luego de haber leído
                // varios caracteres significa que el archivo
                // no tiene un `\n` antes del EOF.
                return maybe_string_of(string);
            }
        }

        char fgetc_success = (char)fgetc_response;

        if (fgetc_success == '\r') {
            continue;
        }

        if (fgetc_success == '\n' || fgetc_success == '\0') {
            return maybe_string_of(string);
        }

        string = string_append_b(
            string_from_c_string(&fgetc_success, 1),
            string
        );
    }

    return maybe_string_of(string);
}

Line line_from_file(FILE *file_mut_borrow, u32 line_number, u32 max_read_length) {
    MaybeString maybe_string = c_line_from_file(file_mut_borrow, max_read_length);

    if (!maybe_string.has_value) {
        maybe_string_destroy(maybe_string);
        return line_eof(line_number);
    }

    String string = maybe_string.value;

    if (string_ends_with_b(string_from_c_string("\n", 1), string_borrow(string))) {
        string = string_substring_b(0, string_length(string_borrow(string)) - 1, string);
    }

    Line line = line_of(
        string,
        line_number
    );

    return line;
}
