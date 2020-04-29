#include <signal.h>
#include <stdbool.h>

#include "GrafoSt2020.h"
#include "array.h"
#include "log.h"

#define HUGE_NUM 1000000

bool TestExpandArray(void) {
    int n = 8;
    u32 *expected = (u32[]){2, 4, 8, 16, 32, 64, 128, 256};

    Array *a = ArrayNew();

    for (int i = 0; i < n; ++i) {
        arrayExpand(a);
        if (a->capacity != expected[i]) {
            log_error("%s: array didn't expand, expected %u, got %u", __func__,
                      expected[i], a->capacity);
            ArrayDestroy(a);
            return false;
        }
    }

    ArrayDestroy(a);
    return true;
}

bool TestArrayAdd(void) {
    Array *a = ArrayNew();
    u32 pos = 0;
    int n = 8;
    u32 *nums = (u32[]){1, 2, 4, 8, 16, 32, 64, 128};

    for (int i = 0; i < n; ++i) {
        pos = ArrayAdd(a, nums[i]);
        log_debug("%s: added %u to array on pos %u", __func__, nums[i], pos);
    }

    ArrayDestroy(a);
    return true;
}

bool TestArrayGet(void) {
    Array *a = ArrayNew();

    for (u32 i = HUGE_NUM; i > 0; --i) {
        u32 pos = ArrayAdd(a, i);
        u32 val = ArrayGet(a, pos);

        if (val != i) {
            log_error("%s: expected %u, got %u", __func__, i, val);
            arrayPrint(a);
            ArrayDestroy(a);
            return false;
        }
    }

    ArrayDestroy(a);
    return true;
}

bool TestFindGap(void) {
    Array *a = ArrayNew();

    u32 res = findGap(a);
    if (res != 0) {
        log_error("%s: findGap should return 0 on no neighbours, got %u",
                  __func__, res);
        ArrayDestroy(a);
        return false;
    }

    u32 nodes[] = {0, 1, 3, 4};
    u32 n_count = 4;
    for (u32 i = 0; i < n_count; i++) ArrayAdd(a, nodes[i]);
    res = findGap(a);
    if (res != 2) {
        log_error("%s: findGap should return 2 on [0, 1, 3, 4], got %u",
                  __func__, res);
        ArrayDestroy(a);
        return false;
    }

    ArrayClean(a);
    u32 nodes_2[] = {UNCOLORED, UNCOLORED, UNCOLORED};
    n_count = 3;
    for (u32 i = 0; i < n_count; i++) ArrayAdd(a, nodes_2[i]);
    res = findGap(a);
    if (res != 0) {
        log_error("%s: findGap should return 0 on uncolored nodes, got %u",
                  __func__, res);
        ArrayDestroy(a);
        return false;
    }

    ArrayClean(a);
    u32 nodes_3[] = {22, 44, 66};
    n_count = 3;
    for (u32 i = 0; i < n_count; i++) ArrayAdd(a, nodes_3[i]);
    res = findGap(a);
    if (res != 0) {
        log_error("%s: findGap should return 0 on [22, 44, 66], got %u",
                  __func__, res);
        ArrayDestroy(a);
        return false;
    }

    ArrayClean(a);
    u32 nodes_4[] = {0, 1, 2};
    n_count = 3;
    for (u32 i = 0; i < n_count; i++) ArrayAdd(a, nodes_4[i]);
    res = findGap(a);
    if (res != 3) {
        log_error("%s: findGap should return 3 on [1, 2, 3], got %u", __func__,
                  res);
        ArrayDestroy(a);
        return false;
    }

    ArrayClean(a);
    u32 nodes_5[] = {0, 1, 1, 1, 2, 3, 6};
    n_count = 7;
    for (u32 i = 0; i < n_count; i++) ArrayAdd(a, nodes_5[i]);
    res = findGap(a);
    if (res != 4) {
        log_error("%s: findGap should return 4 on [0, 1, 1, 1, 2, 3, 6], got %u", __func__,
                  res);
        ArrayDestroy(a);
        return false;
    }

    ArrayDestroy(a);
    return true;
}

bool TestArraySet(void) {
    Array *a = ArrayNew();

    for (u32 i = HUGE_NUM; i > 0; --i) {
        ArrayAdd(a, i);
    }

    for (u32 i = 0; i < a->length; i++) {
        u32 val = i * 2;
        ArraySet(a, i, val);

        u32 setval = ArrayGet(a, i);
        if (setval != val) {
            log_error("%s: expected %u, got %u", __func__, val, setval);
            ArrayDestroy(a);
            return false;
        }
    }

    ArrayDestroy(a);
    return true;
}

bool TestArrayCopy(void) {
    Array *a = ArrayNew();
    for (u32 i = 0; i < 10000; ++i) {
        ArrayAdd(a, i);
    }

    Array *newA = ArrayCopy(a);
    if (newA->length != a->length) {
        log_error("%s: expected length %u, got %u", __func__, a->length,
                  newA->length);
        goto cleanup_false;
    }

    if (newA->capacity != a->capacity) {
        log_error("%s: expected capacity %u, got %u", __func__, a->capacity,
                  newA->capacity);
        goto cleanup_false;
    }

    bool ok = arrayEqual(a, newA);
    if (!ok) {
        goto cleanup_false;
    }

    ArrayDestroy(a);
    ArrayDestroy(newA);
    return true;

cleanup_false:
    ArrayDestroy(a);
    ArrayDestroy(newA);
    return false;
}

bool TestEdgeArrayCreation(void) {
    EdgeArray *e = EdgeArrayNew();
    if (e == NULL) {
        log_error("%s: failed to create edge array.", __func__);
        return false;
    }
    EdgeArrayDestroy(e);
    return true;
}

bool TestEdgeArrayAdd(void) {
    EdgeArray *e = EdgeArrayNew();

    u32 pos = EdgeArrayAdd(e);
    if (pos != 0) {
        log_error("%s: bad position. Expected %u got %u.", __func__, 0, pos);
        EdgeArrayDestroy(e);
        return false;
    }

    pos = EdgeArrayAdd(e);
    if (pos != 1) {
        log_error("%s: bad position. Expected %u got %u.", __func__, 1, pos);
        EdgeArrayDestroy(e);
        return false;
    }

    EdgeArrayDestroy(e);
    return true;
}

bool TestEdgeArrayConnect(void) {
    EdgeArray *e = EdgeArrayNew();

    EdgeArrayAdd(e);
    EdgeArrayAdd(e);
    EdgeArrayConnect(e, 0, 0, 1, 1);
    Array *a = EdgeArrayGetConnections(e, 0);
    if (ArrayGet(a, 0) != 1) {
        log_error("%s: Didn't find connection.", __func__);
        arrayPrint(a);
        EdgeArrayDestroy(e);
        return false;
    }
    a = EdgeArrayGetConnections(e, 1);
    if (ArrayGet(a, 0) != 0) {
        log_error("%s: Didn't find connection.", __func__);
        arrayPrint(a);
        EdgeArrayDestroy(e);
        return false;
    }

    EdgeArrayDestroy(e);
    return true;
}

int main(void) {
    int ntests = 9;

    // array of function pointers - awful syntax, saves plenty of lines
    bool (*tests[])(void) = {
        TestExpandArray,       TestArrayAdd,     TestArrayGet,
        TestArraySet,          TestArrayCopy,    TestFindGap,
        TestEdgeArrayCreation, TestEdgeArrayAdd, TestEdgeArrayConnect};

    for (int i = 0; i < ntests; ++i) {
        bool ok = tests[i]();
        if (!ok) {
            return 1;
        }
    }

    return 0;
}
