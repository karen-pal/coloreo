
#include "Nombres.c"
#include "../assert.h"

bool nombres_test() {
    FILE *file_bxb15_22_10 = fopen("grafos/bxb15_22_10.col", "r");

    Assert(file_bxb15_22_10 != NULL);

    DimacsParseResult parse_result = dimacs_from_file(file_bxb15_22_10);

    fclose(file_bxb15_22_10);

    Assert(parse_result.is_success);

    MaybeSortedSetU32 nombres = nombres_desde_dimacs(dimacs_borrow(parse_result.success));

    dimacs_parse_result_destroy(parse_result);

    Assert(nombres.has_value);

    for (u32 i = 0; i < parse_result.success.numero_de_vertices; i = i + 1) {
        Assert(
            nombre_from_orden_natural(orden_natural_of(i), nombres.value).has_value
        )
        Assert(
            orden_natural_from_nombre(
                nombre_from_orden_natural(orden_natural_of(i), nombres.value).value,
                nombres.value
            ).has_value
        )
        Assert(
            orden_natural_from_nombre(
                nombre_from_orden_natural(orden_natural_of(i), nombres.value).value,
                nombres.value
            ).value.value == i
        )
    }

    sorted_set_u32_destroy(nombres.value);

    EndTest
}

bool nombres_invalido_test() {
    FILE *file_invalido_nro_vertices = fopen("grafos/invalido_nro_vertices.col", "r");

    Assert(file_invalido_nro_vertices != NULL);

    DimacsParseResult parse_result = dimacs_from_file(file_invalido_nro_vertices);

    fclose(file_invalido_nro_vertices);

    Assert(parse_result.is_success);

    MaybeSortedSetU32 nombres = nombres_desde_dimacs(dimacs_borrow(parse_result.success));

    dimacs_parse_result_destroy(parse_result);

    Assert(!nombres.has_value);

    EndTest
}

int main() {
    return nombres_test() && nombres_invalido_test() ? 0 : -1;
}
