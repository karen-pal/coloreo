#ifndef MAYBE_U32
#define MAYBE_U32

#include "../common/common.c"

struct maybe_u32 {
    bool has_value;
    u32 value;
};

typedef struct maybe_u32 maybe_u32;

maybe_u32 maybe_u32_of(u32 value) {
    maybe_u32 maybe = { true, value };
    return maybe;
}

maybe_u32 maybe_u32_none() {
    maybe_u32 maybe = { false, 0 };
    return maybe;
}

#endif
