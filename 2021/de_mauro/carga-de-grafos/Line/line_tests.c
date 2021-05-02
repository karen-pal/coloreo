#include <stdio.h>
#include "Line.c"
#include "../assert.h"

bool test() {
    FILE *file_c_3 = fopen("grafos/C_3.col", "r");

    Line line = line_from_file(file_c_3, 0, 256);
    Assert(
        line.string.has_value
    );
    Assert(
        string_equals_c(
            string_from_c_string("c Ciclo 3", 99),
            line.string.value
        )
    );

    line = line_from_file(file_c_3, 1, 256);
    Assert(
        line.string.has_value
    );
    Assert(
        string_equals_c(
            string_from_c_string("p edge 3 3", 99),
            line.string.value
        )
    );

    fclose(file_c_3);

    EndTest
}

int main() {
    bool result = test();

    return result ? 0 : 1;
}
