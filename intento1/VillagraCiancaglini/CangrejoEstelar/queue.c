#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "array.h"
#include "log.h"
#include "queue.h"

Queue *QueueNew(u32 capacity) {
    Queue *q = calloc(1, sizeof(Queue));
    if (capacity == 0) {
        return q;
    }
    q->contents = calloc(capacity, sizeof(u32));
    q->capacity = capacity;
    return q;
}

void QueueDestroy(Queue *q) {
    if (q == NULL) {
        log_error("sefault: null queue");
        raise(SIGSEGV);
    }
    free(q->contents);
    free(q);
}

void QueueExpand(Queue *q) {
#ifdef NDEBUG
    if (q == NULL) {
        log_error("sefault: null queue");
        raise(SIGSEGV);
    }
#endif
    if (q->start >= q->capacity / 2) {
        u32 *tmp = calloc(q->capacity, sizeof(u32));
        for (u32 i = 0; i < q->size; ++i) {
            tmp[i] = q->contents[i + q->start];
        }
        free(q->contents);
        q->contents = tmp;
        q->start = 0;
        q->end = q->size;
        return;
    }
    log_debug("%s: old array is %zu bytes", __func__,
              q->capacity * sizeof(u32));
    u32 growth = q->capacity < megabyte ? q->capacity * 2
                                        : q->capacity + megabyte;
    u32 ncap = q->capacity == 0 ? 1 : growth;
    q->contents = realloc(q->contents, ncap * sizeof(u32));
    q->capacity = ncap;
    log_debug("%s: new array is %zu bytes", __func__,
              q->capacity * sizeof(u32));
}

void QueueAdd(Queue *q, u32 element) {
#ifndef NDEBUG
    if (q == NULL) {
        log_error("sefault: null queue");
        raise(SIGSEGV);
    }
#endif
    if (q->capacity == q->end) {
        log_debug("%s: expanding queue: len=%u, cap=%u, start=%u, end=%u",
                  __func__, q->size, q->capacity, q->start, q->end);
        QueueExpand(q);
    }
    q->contents[q->end] = element;
    q->end++;
    q->size++;
}

u32 QueuePull(Queue *q) {
#ifndef NDEBUG
    if (q == NULL) {
        log_error("sefault: null queue");
        raise(SIGSEGV);
    }
    if (q->start == q->end) {
        log_error("sefault: empty queue");
        raise(SIGSEGV);
    }
#endif
    u32 rv = q->contents[q->start];
    q->start++;
    q->size--;
    return rv;
}
