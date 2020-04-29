#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "array.h"
#include "base.h"

typedef struct {
    Array *V;  // Buncha names
    Array *Order;
    EdgeArray *E;
    Array *Colors;
    struct {
        u32 key;
        u32 value;
    } * hash;

    u32 nsides;
    u32 delta;
    u32 maxcolor;
} GrafoSt;

GrafoSt *GraphNew(void);
void GraphDestroy(GrafoSt *g);

void GraphAddNode(GrafoSt *g, u32 name);
u32 NameToPos(GrafoSt *g, u32 name);
bool GraphParse(GrafoSt *g, FILE *stream);
void GraphConnectNodes(GrafoSt *g, u32 name1, u32 name2);
void uncolor(GrafoSt *g);
