
#include "Dimacs.c"
#include "../assert.h"

bool dimacs_C_3_test() {
    FILE *file_c_3 = fopen("grafos/C_3.col", "r");

    Assert(file_c_3 != NULL);

    DimacsParseResult parse_result = dimacs_from_file(file_c_3);

    fclose(file_c_3);

    Assert(parse_result.is_success);

    Assert(string_equals_c(
        dimacs_sides_to_string(99, dimacs_borrow(parse_result.success)),
        string_from_c_string("{ 1 → 2, 2 → 3, 3 → 1 }", 99)
    ));

    Assert(string_equals_c(
        dimacs_sides_to_string(3, dimacs_borrow(parse_result.success)),
        string_from_c_string("{ 1 → 2, 2 → 3, 3 → 1 }", 99)
    ));

    Assert(string_equals_c(
        dimacs_sides_to_string(2, dimacs_borrow(parse_result.success)),
        string_from_c_string("{ 1 → 2, 2 → 3, etc }", 99)
    ));

    Assert(string_equals_c(
        dimacs_sides_to_string(1, dimacs_borrow(parse_result.success)),
        string_from_c_string("{ 1 → 2, etc }", 99)
    ));

    dimacs_parse_result_destroy(parse_result);

    EndTest
}

bool dimacs_invalido_0_test() {
    FILE *file_invalido_0 = fopen("grafos/invalido_0.col", "r");

    Assert(file_invalido_0 != NULL);

    DimacsParseResult parse_result = dimacs_from_file(file_invalido_0);

    fclose(file_invalido_0);

    Assert(
        !parse_result.is_success
    );

    Assert(
        parse_result.error.tag == EXPECTING_DIMACS_HEADER
    );

    dimacs_parse_result_destroy(parse_result);

    EndTest
}

bool dimacs_invalido_1_test() {
    FILE *file_invalido_1 = fopen("grafos/invalido_1.col", "r");

    Assert(file_invalido_1 != NULL);

    DimacsParseResult parse_result = dimacs_from_file(file_invalido_1);

    fclose(file_invalido_1);

    Assert(
        !parse_result.is_success
    );

    Assert(
        parse_result.error.tag == INVALID_DIMACS_HEADER
    );
    Assert(
        parse_result.error.line.string.has_value
    );

    dimacs_parse_result_destroy(parse_result);

    EndTest
}

bool dimacs_invalido_2_test() {
    FILE *file_invalido_2 = fopen("grafos/invalido_2.col", "r");

    Assert(file_invalido_2 != NULL);

    DimacsParseResult parse_result = dimacs_from_file(file_invalido_2);

    fclose(file_invalido_2);

    Assert(
        !parse_result.is_success
    );

    Assert(
        parse_result.error.tag == EXPECTING_DIMACS_EDGE
    );

    dimacs_parse_result_destroy(parse_result);

    EndTest
}

bool dimacs_vacio_test() {
    FILE *file_vacio = fopen("grafos/vacio.col", "r");

    Assert(file_vacio != NULL);

    DimacsParseResult parse_result = dimacs_from_file(file_vacio);

    fclose(file_vacio);

    Assert(
        !parse_result.is_success
    );

    Assert(
        parse_result.error.tag == EXPECTING_DIMACS_HEADER
    );
    Assert(
        !parse_result.error.line.string.has_value
    );

    dimacs_parse_result_destroy(parse_result);

    EndTest
}

bool dimacs_bxb15_22_10_test() {
    FILE *file_bxb15_22_10 = fopen("grafos/bxb15_22_10.col", "r");

    Assert(file_bxb15_22_10 != NULL);

    double time0 = get_time();

    DimacsParseResult parse_result = dimacs_from_file(file_bxb15_22_10);

    double time1 = get_time();

    fclose(file_bxb15_22_10);

    Assert(
        parse_result.is_success
    );

    Assert(
        time1 - time0 < 0.4625
    );

    // Estos números son muy específicos de mi computadora.
    // Los tiempos son más rápidos si saco Valgrind de el medio, por ejemplo.
    // Pero está bueno que quede escrito para poder ver si algún cambio en el código afecta esto.
    Assert(
        time1 - time0 > .115 - .020
    );

    // (Sin valgrind me tarda menos de 35 milisegundos)

    dimacs_parse_result_destroy(parse_result);

    EndTest
}

int main() {
    bool result = dimacs_C_3_test();
    result = dimacs_invalido_0_test() && result;
    result = dimacs_invalido_1_test() && result;
    result = dimacs_invalido_2_test() && result;
    result = dimacs_vacio_test() && result;
    result = dimacs_bxb15_22_10_test() && result;

    return result ? 0 : 1;
}
