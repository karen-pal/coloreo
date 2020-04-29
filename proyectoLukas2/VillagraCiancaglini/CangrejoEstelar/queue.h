#pragma once

#include "base.h"

typedef struct {
    u32 *contents;
    u32 start;
    u32 end;
    u32 capacity;
    u32 size;
} Queue;

Queue *QueueNew(u32 capacity);
void QueueDestroy(Queue *q);
void QueueExpand(Queue *q);

void QueueAdd(Queue *q, u32 element);
u32 QueuePull(Queue *q);
