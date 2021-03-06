#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <async/Generator.h>
#include <async/Partial.h>
#include <async/rc.h>
#include <async/test.h>
// http://albertnetymk.github.io/2014/12/05/context/

void count(Generator* gen) {
    for (int i = 0; i < 3; i++) {
        char msg = *((char*)GeneratorYield(gen, (void*)&i));
        //printf("received %c\n", msg);
    }
    GeneratorReturn(gen, NULL);
}


/// GENERATORS FOR TESTS //

void load_images(Generator* gen) {
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = 10000000;
    for (int i = 0; i < 20; i++) {
        //printf("%d images loaded     \r", i);
        fflush(stdout);
        GeneratorYield(gen, NULL);
        nanosleep(&req, NULL);
        GeneratorYield(gen, NULL);
        GeneratorYield(gen, NULL);
        nanosleep(&req, NULL);
    }
}

void load_records(Generator* gen) {
    struct timespec req;
    req.tv_sec = 0;
    req.tv_nsec = 50000000;
    for (int i = 0; i < 15; i++) {
        //printf("\r%d records loaded    ", i);
        fflush(stdout);
        GeneratorYield(gen, NULL);
        nanosleep(&req, NULL);
        nanosleep(&req, NULL);
    }
}

void Gen1337(Generator* gen) {
    static int val = 1337;
    GeneratorReturn(gen, (void*)&val);
}

void EchoOnce(Generator* gen) {
    int a = 1;
    int b = 0;
    b = *((int*)GeneratorYield(gen, (void*)&a));
    GeneratorReturn(gen, &b);
}

void RecvRange(Generator* gen) {
    static int i = 0;
    for (i = 1; i < 6; i++) {
        GeneratorYield(gen, (void*)&i);
    }
}

void SendRecvRange(Generator* gen) {
    int i = 0;
    int tmp = 0;
    int value = 0;
    for (i = 0; i < 5; i++) {
        tmp = i + value;
        value = *(int*)GeneratorYield(gen, (void*)&tmp);
    }
}

void YieldFrom(Generator* gen) {
    void (*other)(Generator*) = (void(*)(Generator*))GeneratorYield(gen, NULL);
    GeneratorYieldFrom(gen, other);
}

/// TEST FUNCTIONS ///

bool test0() {
    printf("should receive a single value");
    Generator* gen = GeneratorCreate(Gen1337);
    int* itval = (int*)GeneratorNext(gen, NULL);
    int val = *itval;
    DONE(gen);
    return val == 1337;
}

bool test1() {
    int val_1 = 1234;
    int val_2 = 5678;
    int *tmp = NULL;
    printf("should echo a single value");

    bool success = true;
    Generator* echo = GeneratorCreate(EchoOnce);

    tmp = (int*)GeneratorNext(echo, (void*)&val_1);
    success = *tmp == 1;

    tmp = (int*)GeneratorNext(echo, (void*)&val_2);
    success = success && *tmp == 5678;

    DONE(echo);
    return success;
}

bool test2() {
    printf("should iterate basic values");

    Generator* numbers = GeneratorCreate(RecvRange);
    int value;
    for (;;) {
        const int* itvalue = (int*)GeneratorNext(numbers, NULL);
        if (numbers->done) {
            break;
        }
        if (itvalue != NULL) {
            value = *itvalue;
        }
    }
    DONE(numbers);
    return value == 5;
}

bool test3() {
    printf("should send/receive during iteration");
    Generator* gen = GeneratorCreate(SendRecvRange);
    int val = 0;
    for (int i = 100; ;i-=2) {
        int* itval = GeneratorNext(gen, &i);
        if (gen->done) {
            break;
        }
        val = *itval;
    }
    DONE(gen);
    return val == 96;
}

bool test4() {
    printf("should yield from");
    int val;
    Generator* gen = GeneratorCreate((void (*)(Generator*))YieldFrom);
    GeneratorNext(gen, NULL);
    GeneratorNext(gen, RecvRange); // send RecvRange as "argument"
    for (;;) {
        int* value = (int*)GeneratorNext(gen, NULL);
        if (gen->done) {
            break;
        }
        val = *value;
    }
    DONE(gen);
    return val == 5;
}

bool test5() {
    printf("should send/receive yield from");
    Generator* gen = GeneratorCreate((void (*)(Generator*))YieldFrom);
    GeneratorNext(gen, NULL);
    GeneratorNext(gen, SendRecvRange);

    int value = 0;
    for (int i = 1; ; i++) {
        int* itval = (int*)GeneratorNext(gen, (void*)&i);
        if (gen->done) {
            break;
        }
        value = *itval;
    }
    DONE(gen);
    return value == 9;
}

bool test6() {
    printf("should demonstrate a scheduler based on generators");

    int num_tasks = 2;
    Generator* tasks[2];
    tasks[0] = GeneratorCreate(load_images);
    tasks[1] = GeneratorCreate(load_records);
    int tasks_done = 0;
    int i;
    for (i = 0; ; i++) {
        const int id = i & 1;
        Generator* task = tasks[id];
        if (task != NULL) {
            if (task->done) {
                DONE(task);
                tasks[id] = NULL;
                tasks_done++;
                if (tasks_done == num_tasks) {
                    break;
                }
            } else {
                GeneratorNext(task, NULL);
            }
        }
    }
    return i == 122;
}


// TODO - make a macro or something so generator details can be elided
// https://stackoverflow.com/questions/62903631/use-c-c-macros-to-generate-function-signature
// ^^ macro examples
void add_numbers(PRT_PARAMETERS) {
    long a = PRT_PARAM(long);
    long b = PRT_PARAM(long);
    PRT_RETURN (a + b);
}

bool test7() {
    printf("should partially apply a function using generators");

    partial_t* add_numbers_p = PartialCreate(add_numbers);
    PRT_APPLY(add_numbers_p, 1L);
    long sum = (long)PRT_APPLY(add_numbers_p, 2L);
    DONE(add_numbers_p); // optional

    return sum == 3;
}

int main() {
    test_preamble();
    test(test0);
    test(test1);
    test(test2);
    test(test3);
    test(test4);
    test(test5);
    //test(test6);
    test(test7);
    return 0;
}
