#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <callback.h>
#include <async/Events.h>
#include <async/Emitter.h>
#include <async/Promise.h>
#include <async/Generator.h>
#include <async/testutils.h>
#include <async/test.h>
#include <async/rc.h>

#define NEW_PROMISE() PromiseCreate(events)
#define RESOLVE(p, data) PromiseResolve(p, data)
#define AWAIT(x) GeneratorYield(gen, x)
#define AWAIT_AS(x, type) ((type)GeneratorYield(gen, x))
#define AWAIT_RETURN(x) GeneratorReturn(gen, x)

typedef struct scope_t {
    Generator* async_routine;
    Promise* master_promise;
} scope_t;

// TODO - part out the functions and structures that don't need to be in here
// TODO - tests, tests, and more tests
// TODO - assume one universal event loop, and therefore simplify promises and event listeners

void resolver(void* ptr) {

    // preamble //
    Promise* old_promise = (Promise*)ptr;
    void* awaited_result = old_promise->value;
    scope_t* scope = old_promise->__misc;

    Generator* async_routine = scope->async_routine;
    Promise* master_promise = scope->master_promise;
    void* iteration_value = GeneratorNext(async_routine, awaited_result);

    if (iteration_value == NULL || async_routine->done) {
        DONE(scope);
        PromiseResolve(master_promise, iteration_value);
    } else {
        Promise* awaited_promise = (Promise*)iteration_value;
        awaited_promise->__misc = scope;
        PromiseThenSelf(awaited_promise, resolver);
    }

}

void scope_finalizer(void* ptr) {
    scope_t* this = (scope_t*)ptr;
    printf("\nfinalizing scope_t\n");
    DONE(this->async_routine);
}

Promise* call_async(void (*func)(Generator*)) {
    Promise* p = PromiseCreate(events);

    Generator* async_routine = GeneratorCreate(func);

    scope_t* scope = NEW(scope_t, scope_finalizer, free);
    scope->async_routine = async_routine;
    scope->master_promise = p;
    p->__misc = scope;

    resolver(p);
    return p;
}


// ================================= //








void test_promise_finalizer(void* ptr) {
    Promise* this = (Promise*)ptr;
    printf("\nfinalizing promise\n");
    PromiseFinalize(this);
}

Promise* new_test_promise() {
    Promise* this = NEW_PROMISE();
    set_finalizer(this, test_promise_finalizer);
    return this;
}


static void resolveRecord(void* prm) {
    RESOLVE((Promise*)prm, NULL);
}

Promise* loadRecord() {
    Promise* record_prm = NEW_PROMISE();
    setTimeout(resolveRecord, record_prm, 200);
    return record_prm;
}

void loadRecords(Generator* gen) {
    static int i = 0;
    for (i = 0; i < 5; i++) {
        Promise* p = loadRecord();
        AWAIT(p);
        DONE(p);
        printf("loaded %d records...\n", i+1);
    }
    printf("done\n");
    AWAIT_RETURN (&i);
}

void main_finish(void* ptr) {
    Promise* p = (Promise*)ptr;
    printf("received records: %d\n", *(int*)(p->value));
    DONE(p);
    stop_event_loop();
}

void hello_world(void* i) {
    printf("hello world %ld!\n", (long)i);
}

void entrypoint(void* data) {
    setTimeout(hello_world, (void*)1L, 500);
    PromiseThenSelf(call_async(loadRecords), main_finish);
}

int main() {
    test_preamble();
    start_event_loop(entrypoint);
    return 0;
}
