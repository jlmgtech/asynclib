#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include "../include/Events.h"
#include "../include/partial.h"
#include "../include/util.h"
#include "../include/generics.h"
#include "../include/Promise.h"

#define test(x) if (x()) {\
    printf(": PASS!\n");\
} else {\
    printf(": FAIL!\n");\
    exit(1);\
}

Events* events;

void callback_0(void* data) {
    int* number = (int*)data;
    *number = *number + 1;
}

bool test0() {
    printf("should schedule promise callback when calling resolve AFTER setting it");

    int data = 1337;
    Promise p;
    PromiseInit(&p, events);
    PromiseThen(&p, callback_0);
    PromiseResolve(&p, &data);
    bool success = data == 1337;
    EventsRun(events);
    success = success && data == 1338;

    return success;
}

bool test1() {
    printf("should schedule promise callback when calling resolve BEFORE setting it");

    int data = 1337;
    Promise p;
    PromiseInit(&p, events);
    PromiseResolve(&p, &data);
    PromiseThen(&p, callback_0);
    bool success = data == 1337;
    EventsRun(events);
    success = success && data == 1338;

    return success;
}

bool test2() {
    printf("should schedule first promise callback after first resolve, and second after second resolve");

    int data = 1337;

    Promise p;
    PromiseInit(&p, events);

    PromiseResolve(&p, &data);
    PromiseThen(&p, callback_0);

    bool success = data == 1337;
    EventsRun(events);
    success = success && data == 1338;

    PromiseThen(&p, callback_0);
    PromiseThen(&p, callback_0);
    EventsRun(events);
    success = success && data == 1340;

    return success;
}

void add_nums(PRT_PARAMETERS) {
    long a = PRT_PARAM(long);
    long b = PRT_PARAM(long);
    long* data = PRT_PARAM(long*);
    *data = a + b;
    PRT_RETURN (NULL);
}

bool test3() {
    printf("should schedule a partially applied function");
    long data = 0L;

    partial_t* partial = make_partial(add_nums);
    PRT_APPLY(partial, 5L);
    PRT_APPLY(partial, 6L);
    EventsPush(events, (void*)partial, (void*)&data);
    EventsRun(events);
    return data == 11L;
}

int main() {
    printf("\n\nsup\n");
    events = EventsCreate();

    test(test0);
    test(test1);
    test(test2);
    test(test3);

    EventsDestroy(events);
    printf("\n\ndone\n");
    return 0;
}
