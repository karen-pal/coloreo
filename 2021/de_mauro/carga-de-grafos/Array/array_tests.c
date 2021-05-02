#include <stdio.h>
#include "Array.c"
#include "../assert.h"

#define UNUSED(x) (void)x
void destroy_u32(void *n) {
    UNUSED(n);
}

bool array_create_test() {
    Array array = array_blank(10, sizeof(u32));

    Assert(array_length(array) == 10);
    Assert(*(u32 *)array_read_at(0, array) == 0);

    array_destroy(destroy_u32, array);

    EndTest;
}

bool array_push_test() {
    Array array = array_blank(10, sizeof(u32));

    Assert(array_length(array) == 10);

    u32 element = 5;

    array = array_push(&element, array);

    Assert(array_length(array) == 11)

    Assert(*(u32 *)array_read_at(10, array) == element);

    array_destroy(destroy_u32, array);

    EndTest;
}

bool array_reverse_test() {
    Array array = array_blank(100, sizeof(u32));

    for (u32 i = 0; i < 100; i++) {
        array = array_write_at(&i, i, array);
    }

    Assert(*(u32 *)array_read_at(0, array) == 0)
    Assert(*(u32 *)array_read_at(49, array) == 49)
    Assert(*(u32 *)array_read_at(99, array) == 99)

    array = array_reverse(array);

    Assert(*(u32 *)array_read_at(0, array) == 99)
    Assert(*(u32 *)array_read_at(49, array) == 50)
    Assert(*(u32 *)array_read_at(99, array) == 0)

    array_destroy(destroy_u32, array);

    EndTest
}

int main() {
    bool result = array_create_test(stdout, stderr)
        && array_push_test(stdout, stderr)
        && array_reverse_test(stdout, stderr);

    if (result) {
        return 0;
    } else {
        return 1;
    }
}
