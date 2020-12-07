#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <async/Emitter.h>
#include <async/test.h>
#include <async/rc.h>

int acc = 0;
int acc2 = 0;
int acc3 = 0;

void callback(void* data) {
    acc = *(int*)data;
}

void callback2(void* data) {
    acc2 = *(int*)data;
}

void callback3(void* data) {
    acc3 = *(int*)data;
}

bool test0() {
    printf("should add a listener for an event emitter that fires on every dispatch:");

    acc = 0;
    acc2 = 0;

    int value = 1337;
    int* data = &value;

    Emitter* emitter = EmitterCreate();
    EmitterOn(emitter, "ready", callback);
    EmitterEmit(emitter, "ready", (void*)data);
    DONE(emitter);

    return acc == 1337;
}

bool test1() {
    printf("should add multiple listeners that persist");

    acc = 0;
    acc2 = 0;

    int value = 1337;
    int* data = &value;

    Emitter* emitter = EmitterCreate();
    int id_0 = EmitterOn(emitter, "ready", callback);
    int id_1 = EmitterOn(emitter, "ready", callback2);
    EmitterEmit(emitter, "ready", (void*)data);
    DONE(emitter);

    return acc == 1337 && acc2 == 1337;
}

bool test2() {
    printf("should remove a listener");
    acc = 0;
    acc2 = 0;

    int value = 1337;
    int* data = &value;

    Emitter* emitter = EmitterCreate();

    size_t id_1 = EmitterOn(emitter, "ready", callback);
    size_t id_2 = EmitterOn(emitter, "ready", callback2);
    EmitterRemoveListener(emitter, "ready", id_1);
    EmitterEmit(emitter, "ready", (void*)data);

    DONE(emitter);

    return acc == 0 && acc2 == 1337;
}

bool test3() {
    printf("should remove a listener");
    acc = 0;
    acc2 = 0;

    int value = 1337;
    int* data = &value;

    Emitter* emitter = EmitterCreate();

    size_t id_1 = EmitterOn(emitter, "ready", callback);
    size_t id_2 = EmitterOn(emitter, "ready", callback2);

    EmitterRemoveListener(emitter, "ready", id_1);
    EmitterEmit(emitter, "ready", (void*)data);

    DONE(emitter);

    return acc == 0 && acc2 == 1337;
}

bool test4() {
    printf("should add a listener that persists and one that fires only once");
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
    DONE(emitter);

    return success;
}

bool test5() {
    printf("should schedule listeners for different channels");
    acc = 0;
    acc2 = 0;

    int leet_val = 1337;
    int feebee_val = 0xFEEBEE;
    int* leet = &leet_val;
    int* feebee = &feebee_val;

    Emitter* emitter = EmitterCreate();

    size_t id_1 = EmitterOn(emitter, "set 1337", callback);
    size_t id_2 = EmitterOn(emitter, "set 1337", callback2);
    size_t id_3 = EmitterOn(emitter, "set 0xFEEBEE", callback3);

    EmitterRemoveListener(emitter, "ready", id_2);
    EmitterEmit(emitter, "set 1337", (void*)leet);
    EmitterEmit(emitter, "set 0xFEEBEE", (void*)feebee);

    DONE(emitter);
    return acc == 0 && acc2 == 1337 && acc3 == 0xFEEBEE;
}

int main() {
    test_preamble();
    test(test0);
    test(test1);
    test(test2);
    test(test3);
    return 0;
}
