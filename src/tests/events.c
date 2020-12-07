#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>
#include <async/Events.h>
#include <async/Partial.h>
#include <async/util.h>
#include <async/generics.h>
#include <async/Promise.h>
#include <async/rc.h>
#include <async/test.h>

Events* events;

void callback_0(void* data) {
    int* number = (int*)data;
    *number = *number + 1;
}

bool test0() {
    printf("should schedule promise callback when calling resolve AFTER setting it");

    int data = 1337;
    Promise* p = PromiseCreate(events);
    PromiseThen(p, callback_0);
    PromiseResolve(p, &data);
    bool success = data == 1337;
    EventsRun(events);
    success = success && data == 1338;
    DONE(p);

    return success;
}

bool test1() {
    printf("should schedule promise callback when calling resolve BEFORE setting it");

    int data = 1337;
    Promise* p = PromiseCreate(events);
    PromiseResolve(p, &data);
    PromiseThen(p, callback_0);
    bool success = data == 1337;
    EventsRun(events);
    DONE(p);
    success = success && data == 1338;

    return success;
}

bool test2() {
    printf("should schedule first promise callback after first resolve, and second after second resolve");

    int data = 1337;

    Promise* p = PromiseCreate(events);
    PromiseResolve(p, &data);
    PromiseThen(p, callback_0);

    bool success = data == 1337;
    EventsRun(events);
    success = success && data == 1338;

    PromiseThen(p, callback_0);
    PromiseThen(p, callback_0);
    EventsRun(events);
    success = success && data == 1340;
    DONE(p);

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

    partial_t* partial = PartialCreate(add_nums);
    PRT_APPLY(partial, 5L);
    PRT_APPLY(partial, 6L);
    EventsPush(events, (void*)partial, (void*)&data);
    EventsRun(events);
    DONE(partial);
    return data == 11L;
}

int main() {
    test_preamble();
    events = EventsCreate();
    test(test0);
    test(test1);
    test(test2);
    test(test3);
    DONE(events);
    return 0;
}
