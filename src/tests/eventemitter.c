#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../include/Emitter.h"

#define test(x) if (x()) { \
    printf("PASS\n"); \
} else { \
    printf("FAIL\n\n"); \
    exit(1); \
}

int acc = 0;
int acc2 = 0;

void callback(void* data) {
    acc = *(int*)data;
}

void callback2(void* data) {
    acc2 = *(int*)data;
}

bool test0() {
    printf("should add a listener for an event emitter that fires on every dispatch: ");

    acc = 0;
    acc2 = 0;

    int value = 1337;
    int* data = &value;

    Emitter* emitter = EmitterCreate();
    EmitterOn(emitter, "ready", callback);
    EmitterEmit(emitter, "ready", (void*)data);
    EmitterDestroy(emitter);

    return acc == 1337;
}

bool test1() {
    printf("should add multiple listeners that persist: ");

    acc = 0;
    acc2 = 0;

    int value = 1337;
    int* data = &value;

    Emitter* emitter = EmitterCreate();
    int id_0 = EmitterOn(emitter, "ready", callback);
    int id_1 = EmitterOn(emitter, "ready", callback2);
    EmitterEmit(emitter, "ready", (void*)data);
    EmitterDestroy(emitter);

    return acc == 1337 && acc2 == 1337;
}

bool test2() {
    printf("should remove a listener: ");
    acc = 0;
    acc2 = 0;

    int value = 1337;
    int* data = &value;

    Emitter* emitter = EmitterCreate();

    size_t id_1 = EmitterOn(emitter, "ready", callback);
    size_t id_2 = EmitterOn(emitter, "ready", callback2);
    EmitterRemoveListener(emitter, "ready", id_1);

    EmitterEmit(emitter, "ready", (void*)data);
    EmitterDestroy(emitter);

    return acc == 0 && acc2 == 1337;
}

bool test3() {
    printf("should add a listener that persists and one that fires only once: ");
    acc = 0;
    acc2 = 0;

    int value = 1337;
    int* data = &value;

    Emitter* emitter = EmitterCreate();

    size_t id_1 = EmitterOn(emitter, "ready", callback);
    size_t id_2 = EmitterOnce(emitter, "ready", callback2);

    EmitterEmit(emitter, "ready", (void*)data);
    bool success = acc == 1337 && acc2 == 1337;

    acc = 0;
    acc2 = 0;
    EmitterEmit(emitter, "ready", (void*)data);
    success = success && acc == 1337 && acc2 == 0;
    EmitterDestroy(emitter);

    return success;
}

int main() {
    printf("\nrunning tests\n\n");

    test(test0);
    test(test1);
    test(test2);
    test(test3);

    printf("\n\nSUCCESS! all tests PASSED\n\n");
    return 0;
}
