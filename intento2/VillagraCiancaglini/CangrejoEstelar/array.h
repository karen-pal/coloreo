#pragma once

#include <stdbool.h>

#include "base.h"

typedef struct {
    u32 length;
    u32 capacity;
    u32 *contents;
} Array;

bool arrayEqual(Array *old, Array *new);
void arraySort(Array *a);
void arrayExpand(Array *a);
void arrayPrint(Array *a);
char *arrayString(Array *a, u32 n);

Array *ArrayNew(void);
Array *ArrayNewSized(u32 capacity);
void ArrayDestroy(Array *a);
u32 ArrayAdd(Array *a, u32 value);
u32 ArrayGet(Array *a, u32 pos);
void ArraySet(Array *a, u32 pos, u32 value);
Array *ArrayCopy(Array *a);
void ArrayClean(Array *a);

u32 findGap(Array *neighbors);

typedef struct {
    u32 length;
    Array **contents;
} EdgeArray;

/*
  E = {
    0: [1, 0],
    1: [0],
    2: [0],
    3: []
  }
*/

EdgeArray *EdgeArrayNew(void);
void EdgeArrayDestroy(EdgeArray *e);
EdgeArray *EdgeArrayCopy(EdgeArray *e);

void EdgeArrayPrint(EdgeArray *e);
u32 EdgeArrayAdd(EdgeArray *e);
void EdgeArrayConnect(EdgeArray *e, u32 node1, u32 pos1,
                      u32 node2, u32 pos2);
Array *EdgeArrayGetConnections(EdgeArray *e, u32 node);
