#ifndef DIMACS
#define DIMACS

#include <stdio.h>
#include "../Line/Line.c"
#include "../StringArray/StringArray.c"
#include "../u32/pair_u32.c"
#include "../SortedSetU32/SortedSetU32.c"
#include "../Tipos/Tipos.c"

// --- dimacs

/**
 * Representa la información obtenida de un archivo con formato dimacs (sin validar el número de vértices).
 */
struct Dimacs {
    u32 numero_de_vertices;
    u32 numero_de_lados;
    pair_u32 *lados;
};

typedef struct Dimacs Dimacs;

static Dimacs dimacs_of(u32 numero_de_vertices, u32 numero_de_lados, pair_u32 *lados) {
    Dimacs dimacs = { numero_de_vertices, numero_de_lados, lados };
    return dimacs;
}

void dimacs_destroy(Dimacs dimacs) {
    free(dimacs.lados);
}

typedef
    struct dimacs_borrowed {
        Dimacs borrow;
    }
    dimacs_borrowed;

dimacs_borrowed dimacs_borrow(Dimacs dimacs) {
    dimacs_borrowed borrow = { dimacs };
    return borrow;
}

maybe_pair_u32 dimacs_lado(u32 i, dimacs_borrowed dimacs) {
    if (i < dimacs.borrow.numero_de_lados) {
        return maybe_pair_u32_of(dimacs.borrow.lados[i]);
    } else {
        return maybe_pair_u32_none();
    }
}

// --- helpers

static maybe_pair_u32 maybe_pair_u32_from_strings(string_borrowed first, string_borrowed second) {
    maybe_u32 first_number = string_to_u32(first);
    maybe_u32 second_number = string_to_u32(second);

    if (first_number.has_value && second_number.has_value) {
        return maybe_pair_u32_of(pair_u32_of(first_number.value, second_number.value));
    }

    return maybe_pair_u32_none();
}

static maybe_pair_u32 dimacs_header_from_line(line_borrowed line) {
    if (!line.string.has_value) {
        return maybe_pair_u32_none();
    }

    StringArray words = string_split_b(
        string_from_c_string(" ", 1),
        line.string.value
    );

    if (string_array_length(words) >= 4) {
        bool first_word_is_p = string_equals_b(
            string_from_c_string("p", 1),
            string_array_read_at(0, words)
        );

        bool second_word_is_edge = string_equals_b(
            string_from_c_string("edge", 9),
            string_array_read_at(1, words)
        );

        if (first_word_is_p && second_word_is_edge) {
            maybe_pair_u32 pair = maybe_pair_u32_from_strings(
                string_array_read_at(2, words),
                string_array_read_at(3, words)
            );
            string_array_destroy(words);
            return pair;
        }
    }

    string_array_destroy(words);
    return maybe_pair_u32_none();
}

static maybe_pair_u32 dimacs_edge_from_line(line_borrowed line) {
    if (!line.string.has_value) {
        return maybe_pair_u32_none();
    }

    StringArray words = string_split_b(
        string_from_c_string(" ", 1),
        line.string.value
    );

    if (string_array_length(words) >= 3) {
        bool first_word_is_e = string_equals_b(
            string_from_c_string("e", 1),
            string_array_read_at(0, words)
        );

        if (first_word_is_e) {
            maybe_pair_u32 maybe_pair = maybe_pair_u32_from_strings(
                string_array_read_at(1, words),
                string_array_read_at(2, words)
            );

            string_array_destroy(words);
            return maybe_pair;
        }
    }

    string_array_destroy(words);
    return maybe_pair_u32_none();
}

// --- parse result

enum dimacs_parse_error_tag {
    EXPECTING_DIMACS_HEADER,
    INVALID_DIMACS_HEADER,
    EXPECTING_DIMACS_EDGE
};

typedef enum dimacs_parse_error_tag dimacs_parse_error_tag;

struct DimacsParseResult {
    bool is_success;
    union {
        Dimacs success;
        struct {
            dimacs_parse_error_tag tag;
            Line line;
        } error;
    };
};

typedef struct DimacsParseResult DimacsParseResult;

static DimacsParseResult dimacs_parse_success(Dimacs dimacs) {
    DimacsParseResult result;
    result.is_success = true;
    result.success = dimacs;
    return result;
}

static DimacsParseResult dimacs_parse_error(dimacs_parse_error_tag tag, Line line) {
    DimacsParseResult result;
    result.is_success = false;
    result.error.tag = tag;
    result.error.line = line;
    return result;
}

Dimacs dimacs_parse_result_to_dimacs(DimacsParseResult result) {
    if (!result.is_success) {
        fprintf(stderr, "Invalid dimacs_parse_result_to_dimacs call\n");
        exit(-1);
    }
    return result.success;
}

void dimacs_parse_result_destroy(DimacsParseResult result) {
    if (result.is_success) {
        dimacs_destroy(result.success);
    } else {
        line_destroy(result.error.line);
    }
}

// --- parse

static DimacsParseResult dimacs_parse_header_and_body(Line line_header, FILE *file) {
    if (!line_header.string.has_value) {
        return dimacs_parse_error(EXPECTING_DIMACS_HEADER, line_header);
    }

    maybe_pair_u32 header = dimacs_header_from_line(line_borrow(line_header));

    if (!header.has_value) {
        return dimacs_parse_error(EXPECTING_DIMACS_HEADER, line_header);
    }

    if (header.value.first == 0 || header.value.second == 0) {
        return dimacs_parse_error(INVALID_DIMACS_HEADER, line_header);
    }

    u32 line_header_number = line_header.number;

    line_destroy(line_header);

    u32 numero_de_vertices = header.value.first;
    u32 numero_de_lados = header.value.second;

    pair_u32 *lados = (pair_u32 *)safe_calloc(numero_de_lados, sizeof(pair_u32));

    for (u32 i = 0; i < numero_de_lados; i = i + 1) {
        Line line = line_from_file(file, line_header_number + i, 1024);

        maybe_pair_u32 dimacs_edge = dimacs_edge_from_line(line_borrow(line));

        if (dimacs_edge.has_value) {
            line_destroy(line);
            lados[i] = dimacs_edge.value;
        } else {
            free(lados);
            return dimacs_parse_error(EXPECTING_DIMACS_EDGE, line);
        }
    }

    return dimacs_parse_success(dimacs_of(numero_de_vertices, numero_de_lados, lados));
}

DimacsParseResult dimacs_from_file(FILE *file_mut_borrow) {
    for (
        u32 line_number = 0;
        true;
        line_number = line_number + 1
    ) {
        Line line = line_from_file(file_mut_borrow, line_number, 1024);

        if (!line.string.has_value) {
            return dimacs_parse_error(EXPECTING_DIMACS_HEADER, line);
        }

        bool line_is_comment = string_starts_with_b(
            string_from_c_string("c", 1),
            string_borrow(line.string.value)
        );

        if (line_is_comment) {
            line_destroy(line);
        } else {
            return dimacs_parse_header_and_body(line, file_mut_borrow);
        }
    }
}

static String dimacs_side_to_string(pair_u32 side) {
    return string_append_b(
        string_from_u32(side.second),
        string_append_b(
            string_from_c_string(" → ", 9),
            string_from_u32(side.first)
        )
    );
}

/// Example: dimacs_sides_to_string(3, dimacs) may return "{ 1 → 2, 1 → 3, 1 → 4, etc }"
String dimacs_sides_to_string(u32 limit_sides, dimacs_borrowed dimacs) {
    StringArray sides = string_array_empty();

    for (
        u32 i = 0;
        i < limit_sides && i < dimacs.borrow.numero_de_lados;
        i = i + 1
    ) {
        sides = string_array_push(dimacs_side_to_string(dimacs.borrow.lados[i]), sides);
    }

    if (limit_sides < dimacs.borrow.numero_de_lados) {
        sides = string_array_push(
            string_from_c_string("etc", 99),
            sides
        );
    }

    String result = string_from_c_string("{ ", 2);

    result = string_append_b(
        string_array_join_b(
            string_from_c_string(", ", 99),
            sides
        ),
        result
    );

    result = string_append_b(
        string_from_c_string(" }", 99),
        result
    );

    string_array_destroy(sides);

    return result;
}

#endif
