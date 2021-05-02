#include <stdio.h>
#include "StringArray.c"
#include "../assert.h"

bool string_array_push_test() {
    StringArray array = string_array_empty();

    Assert(string_array_length(array) == 0);

    String string = string_from_c_string("hola", 4);

    array = string_array_push(string_copy(string_borrow(string)), array);

    Assert(string_array_length(array) == 1)

    Assert(string_equals_b(string, string_array_read_at(0, array)));

    string_array_destroy(array);

    EndTest;
}

bool string_split_test() {
    StringArray array = string_split_c(
        string_from_c_string(",", 1),
        string_from_c_string("manzana,banana,frutilla,", 99)
    );

    Assert(string_array_length(array) == 4);
    Assert(
        string_equals_b(
            string_from_c_string("manzana", 99),
            string_array_read_at(0, array)
        )
    );
    Assert(
        string_equals_b(
            string_from_c_string("banana", 99),
            string_array_read_at(1, array)
        )
    );
    Assert(
        string_equals_b(
            string_from_c_string("frutilla", 99),
            string_array_read_at(2, array)
        )
    );
    Assert(
        string_equals_b(
            string_empty(),
            string_array_read_at(3, array)
        )
    );

    string_array_destroy(array);

    EndTest
}

bool string_split_test_b() {
    StringArray array = string_split_c(
        string_from_c_string(",", 99),
        string_from_c_string("manzana,banana,frutilla", 99)
    );

    Assert(
        string_array_length(array) == 3
    );
    Assert(
        string_equals_b(
            string_from_c_string("manzana", 99),
            string_array_read_at(0, array)
        )
    );
    Assert(
        string_equals_b(
            string_from_c_string("banana", 99),
            string_array_read_at(1, array)
        )
    );
    Assert(
        string_equals_b(
            string_from_c_string("frutilla", 99),
            string_array_read_at(2, array)
        )
    );

    string_array_destroy(array);

    EndTest
}

int main() {
    bool result = string_array_push_test()
        && string_split_test()
        && string_split_test_b();

    if (result) {
        return 0;
    } else {
        return 1;
    }
}
