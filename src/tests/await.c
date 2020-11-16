#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <callback.h>
#include "../include/Events.h"
#include "../include/Emitter.h"
#include "../include/Promise.h"
#include "../include/Generator.h"
#include "../include/testutils.h"
#define then(p, callback) PromiseThen(p, callback)
#define resolve(p, data) PromiseResolve(p, data)
#define new_Promise() PromiseCreate(events)
#define await(x) GeneratorYield(gen, x)
#define await_return(x) GeneratorReturn(gen, x)

typedef struct scope_t {
    Generator* async_routine;
    Promise* master_promise;
} scope_t;

// TODO - part out the functions and structures that don't need to be in here
// TODO - tests, tests, and more tests
// TODO - call_async and await macros
// TODO - assume one universal event loop, and therefore simplify promises and event listeners
// TODO - collect the garbage - maybe you should just use GC_MALLOC...
// TODO - if not using a gc, use an RC or similar strategy for collecting
//        promises at least (since you won't be expected to keep a reference of
//        it around)
// TODO - make easier callbacks (perhaps using makecontext?)

void resolver(void* old_promise) {

    // preamble //
    void* awaited_result = ((Promise*)old_promise)->value;
    scope_t* scope = ((Promise*)old_promise)->__misc;
    // TODO - you can free the old promise now, I think...
    Generator* async_routine = scope->async_routine;
    Promise* master_promise = scope->master_promise;

    void* iteration_value = GeneratorNext(async_routine, awaited_result);

    if (iteration_value == NULL || async_routine->done) {
        PromiseResolve(master_promise, iteration_value);
    } else {
        Promise* awaited_promise = (Promise*)iteration_value;
        awaited_promise->__misc = scope;
        PromiseThenSelf(awaited_promise, resolver);
    }

}

Promise* call_async(void (*func)(Generator*)) {
    Promise* p = PromiseCreate(events);
    Generator* async_routine = GeneratorMake(func);

    scope_t* scope = malloc(sizeof(scope_t));
    scope->async_routine = async_routine;
    scope->master_promise = p;
    p->__misc = scope;

    resolver(p);
    return p;
}


// ================================= //












static void resolveRecord(void* prm) {
    resolve((Promise*)prm, NULL);
}

Promise* loadRecord() {
    Promise* record_prm = new_Promise();
    setTimeout(resolveRecord, record_prm, 500);
    return record_prm;
}

void loadRecords(Generator* gen) {
    static int i = 0;
    for (i = 0; i < 5; i++) {
        await (loadRecord());
        printf("loaded %d records...\n", i+1);
    }
    printf("done\n");
    await_return (&i);
}

void main_finish(void* v_result) {
    printf("received records: %d\n", *(int*)v_result);
    stop_event_loop();
}

void hello_world(void* i) {
    printf("hello world %ld!\n", (long)i);
}

void entrypoint(void* data) {

    setTimeout(hello_world, (void*)1L, 1700);
    then(call_async(loadRecords), main_finish);
    setTimeout(hello_world, (void*)2L, 1000);

}

int main() {
    start_event_loop(entrypoint);
    return 0;
}
