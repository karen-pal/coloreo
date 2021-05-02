
#include <stdio.h>
#include "String.c"
#include "../assert.h"

bool string_empty_test() {
    String s = string_empty();

    Assert(string_length(string_borrow(s)) == 0);

    string_destroy(s);

    EndTest
}

bool string_from_c_string_test() {
    String s = string_from_c_string("123456789", 99);

    Assert(string_length(string_borrow(s)) == 9);

    // Debería ignorar lo que está después del '\0'
    String p = string_from_c_string("123456789" "\0" "123456789", 99);

    Assert(string_equals_b(s, string_borrow(p)));

    string_destroy(p);

    EndTest
}

bool string_equals_test() {
    String three = string_from_c_string("3", 1);
    String space = string_from_c_string(" ", 1);

    Assert(!string_equals_b(three, string_borrow(space)));

    string_destroy(space);

    EndTest;
}

bool string_copy_test() {
    String s = string_from_c_string("Hola mundo", 99);

    Assert(string_equals_b(string_copy(string_borrow(s)), string_borrow(s)));

    string_destroy(s);

    EndTest
}

bool string_append_test() {
    String hola = string_from_c_string("Hola ", 99);
    String mundo = string_from_c_string("mundo", 99);
    String hola_mundo = string_from_c_string("Hola mundo", 99);

    String hola_mundo_2 = string_append(string_borrow(mundo), string_copy(string_borrow(hola)));

    Assert(string_equals(string_borrow(hola_mundo), string_borrow(hola_mundo_2)));
    Assert(string_starts_with(string_borrow(hola), string_borrow(hola_mundo_2)))

    string_destroy(hola);
    string_destroy(mundo);
    string_destroy(hola_mundo);
    string_destroy(hola_mundo_2);

    EndTest
}

bool string_index_of_test() {
    String hola_mundo = string_from_c_string("Hola mundo", 99);
    String hola = string_from_c_string("Hola", 9);
    String ola = string_from_c_string("ola", 9);
    String chau = string_from_c_string("chau", 9);

    Assert(string_index_of(string_borrow(hola_mundo), string_borrow(hola_mundo)) == 0);
    Assert(string_index_of(string_borrow(hola), string_borrow(hola_mundo)) == 0);
    Assert(string_index_of(string_borrow(ola), string_borrow(hola_mundo)) == 1);
    Assert(string_index_of(string_borrow(chau), string_borrow(hola_mundo)) == -1);

    string_destroy(hola_mundo);
    string_destroy(hola);
    string_destroy(ola);
    string_destroy(chau);

    String comma = string_from_c_string(",", 1);
    String manzana_ = string_from_c_string("manzana,", 99);
    String empty = string_empty();

    Assert(string_starts_with(string_borrow(empty), string_borrow(manzana_)));
    Assert(string_index_of(string_borrow(empty), string_borrow(manzana_)) == 0);
    Assert(string_index_of(string_borrow(comma), string_borrow(manzana_)) == 7);

    string_destroy(empty);
    string_destroy(comma);
    string_destroy(manzana_);

    String three = string_from_c_string("3", 1);
    String space = string_from_c_string(" ", 1);

    Assert(string_index_of(string_borrow(space), string_borrow(three)) == -1);
    Assert(string_index_of(string_borrow(three), string_borrow(three)) == 0);

    string_destroy(three);
    string_destroy(space);

    EndTest;
}

bool string_to_u32_test() {
    String empty = string_empty();
    String menos_uno = string_from_c_string("-1", 9);
    String con_espacio = string_from_c_string("123 ", 9);

    Assert(string_to_u32(string_borrow(empty)).has_value == false);
    Assert(string_to_u32(string_borrow(menos_uno)).has_value == false);
    Assert(string_to_u32(string_borrow(con_espacio)).has_value == false);

    string_destroy(empty);
    string_destroy(menos_uno);
    string_destroy(con_espacio);

    //

    String cero = string_from_c_string("0", 9);
    String cien = string_from_c_string("172", 9);
    String mil = string_from_c_string("9830", 9);

    Assert(string_to_u32(string_borrow(cero)).has_value);
    Assert(string_to_u32(string_borrow(cero)).value == 0);

    Assert(string_to_u32(string_borrow(cien)).has_value);
    Assert(string_to_u32(string_borrow(cien)).value == 172);

    Assert(string_to_u32(string_borrow(mil)).has_value);
    Assert(string_to_u32(string_borrow(mil)).value == 9830);

    string_destroy(cero);
    string_destroy(cien);
    string_destroy(mil);

    EndTest;
}

bool string_ends_with_test() {
    String empty = string_empty();
    String hola = string_from_c_string("hola", 9);
    String a = string_from_c_string("a", 1);
    String b = string_from_c_string("b", 1);

    Assert(string_ends_with(string_borrow(a), string_borrow(hola)));
    Assert(string_ends_with(string_borrow(empty), string_borrow(hola)));
    Assert(string_ends_with(string_borrow(hola), string_borrow(hola)));
    Assert(!string_ends_with(string_borrow(b), string_borrow(hola)));

    string_destroy(empty);
    string_destroy(hola);
    string_destroy(a);
    string_destroy(b);

    EndTest;
}

int main() {
    bool result = string_empty_test();
    result = string_from_c_string_test() && result;
    result = string_equals_test() && result;
    result = string_copy_test() && result;
    result = string_append_test() && result;
    result = string_index_of_test() && result;
    result = string_to_u32_test() && result;
    result = string_ends_with_test() && result;

    if (result) {
        return 0;
    } else {
        return 1;
    }
}
