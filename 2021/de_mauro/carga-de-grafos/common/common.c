#ifndef COMMON_DEFINITIONS
#define COMMON_DEFINITIONS

#include "common.h"

u32 min(u32 a, u32 b) {
    return a < b ? a : b;
}

u32 max(u32 a, u32 b) {
    return a < b ? b : a;
}

#endif
