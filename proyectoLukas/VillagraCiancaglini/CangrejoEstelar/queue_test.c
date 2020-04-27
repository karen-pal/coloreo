#include <signal.h>
#include <stdbool.h>

#include "log.h"
#include "queue.h"

#define BIG_NUM 1000000

bool TestCreateEmptyQueue(void) {
    Queue *q = QueueNew(0);
    if (q == NULL) {
        log_error("%s: queue failed to initialize", __func__);
        return false;
    }
    QueueDestroy(q);
    return true;
}

bool TestCreateBigQueue(void) {
    Queue *q = QueueNew(1789243);
    if (q == NULL) {
        log_error("%s: queue failed to initialize", __func__);
        return false;
    }
    QueueDestroy(q);
    return true;
}

bool TestBigQueue(void) {
    Queue *q = QueueNew(megabyte);
    for (u32 i = 0; i < 3*megabyte; i++) {
        QueueAdd(q, i);
    }
    if (q->capacity > megabyte * 4) {
        log_error("%s: queue grew exponentially to %u", q->capacity);
	QueueDestroy(q);
	return false;
    }
    QueueDestroy(q);
    return true;
}

bool TestEnqueue(void) {
    Queue *q = QueueNew(2);
    if (q == NULL) {
        log_error("%s: queue failed to initialize", __func__);
        return false;
    }

    QueueAdd(q, 1);
    QueueAdd(q, 2);
    u32 res = QueuePull(q);
    if (res != 1) {
        log_error("%s: bad element: got %u expected 1", __func__, res);
        QueueDestroy(q);
        return false;
    }
    res = QueuePull(q);
    if (res != 2) {
        log_error("%s: bad element: got %u expected 2", __func__, res);
        QueueDestroy(q);
        return false;
    }

    QueueDestroy(q);
    return true;
}

bool TestLotsOfElements(void) {
    Queue *q = QueueNew(2);
    if (q == NULL) {
        log_error("%s: queue failed to initialize", __func__);
        return false;
    }

    for (u32 i = 0; i < BIG_NUM; i++) {
        QueueAdd(q, i);
    }

    u32 value = -1;
    for (u32 i = 0; i < BIG_NUM; i++) {
        value = QueuePull(q);
        if (value != i) {
            log_error("%s: bad value: expected %u got %u", __func__, i, value);
            QueueDestroy(q);
            return false;
        }
    }

    QueueDestroy(q);
    return true;
}

bool TestMemoryMaintenance(void) {
    Queue *q = QueueNew(BIG_NUM);
    if (q == NULL) {
        log_error("%s: queue failed to initialize", __func__);
        return false;
    }

    for (u32 j = 0; j < 5; j++) {
        for (u32 i = 0; i < BIG_NUM; i++) {
            QueueAdd(q, i);
        }
        for (u32 i = 0; i < BIG_NUM; i++) {
            QueuePull(q);
        }
    }

    if (q->capacity != BIG_NUM) {
        log_error("%s: queue capacity grew, expected %u got %u.", __func__,
                  BIG_NUM, q->capacity);
        return false;
    }

    QueueDestroy(q);
    return true;
}

int main(void) {
    int ntests = 6;

    // array of function pointers - awful syntax, saves plenty of lines
    bool (*tests[])() = {TestCreateEmptyQueue, TestCreateBigQueue, TestEnqueue,
                         TestLotsOfElements, TestMemoryMaintenance, TestBigQueue};

    for (int i = 0; i < ntests; ++i) {
        bool ok = tests[i]();
        if (!ok) {
            return 1;
        }
    }

    return 0;
}
