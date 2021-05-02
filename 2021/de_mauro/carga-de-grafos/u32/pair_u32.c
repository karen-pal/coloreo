#ifndef U32_PAIR
#define U32_PAIR

#include "../common/common.c"

struct pair_u32 {
    u32 first;
    u32 second;
};

typedef struct pair_u32 pair_u32;

pair_u32 pair_u32_of(u32 first, u32 second) {
    pair_u32 pair = { first, second };
    return pair;
}

struct maybe_pair_u32 {
    bool has_value;
    pair_u32 value;
};

typedef struct maybe_pair_u32 maybe_pair_u32;

maybe_pair_u32 maybe_pair_u32_of(pair_u32 pair) {
    maybe_pair_u32 pair_;
    pair_.has_value = true;
    pair_.value = pair;
    return pair_;
}

maybe_pair_u32 maybe_pair_u32_none() {
    maybe_pair_u32 pair;
    pair.has_value = false;
    return pair;
}

#endif
