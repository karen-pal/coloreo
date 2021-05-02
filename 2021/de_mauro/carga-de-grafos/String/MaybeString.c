#ifndef MAYBE_STRING
#define MAYBE_STRING

#include "String.c"

struct MaybeString {
    bool has_value;
    String value;
};

typedef struct MaybeString MaybeString;

MaybeString maybe_string_of(String string) {
    MaybeString maybe;
    maybe.has_value = true;
    maybe.value = string;
    return maybe;
}

MaybeString maybe_string_none() {
    MaybeString maybe;
    maybe.has_value = false;
    return maybe;
}

void maybe_string_destroy(MaybeString maybe) {
    if (maybe.has_value) {
        string_destroy(maybe.value);
    }
}

/**
 * Por la forma en que está definido, pensarlo como un
 * `Maybe<borrowed<String>>` (en vez de un `borrowed<Maybe<String>>`).
*/
typedef
    struct maybe_string_borrowed {
        bool has_value;
        string_borrowed value;
    }
    maybe_string_borrowed;

maybe_string_borrowed maybe_string_borrow(MaybeString maybe) {
    maybe_string_borrowed borrow;
    borrow.has_value = maybe.has_value;
    borrow.value = string_borrow(maybe.value);
    return borrow;
}

#endif
