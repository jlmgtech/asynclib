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
#define yield(x) GeneratorYield(gen, x)
#define yield_return(x) GeneratorReturn(gen, x)

typedef struct scope_t {
    Generator* async_routine;
    Promise* master_promise;
} scope_t;


// TODO - part out the functions and structures that don't need to be in here
// TODO - tests, tests, and more tests
// TODO - async and await macros
// TODO - assume one universal event loop, and therefore simplify promises and event listeners
// TODO - simplify "above the hood" function signatures to make global assumptions about stuff
// TODO - collect the garbage - maybe you should just use gc_malloc...
// TODO - if not using a gc, use an RC or similar strategy for nullifying
//        promises at least (since you won't be expected to keep a reference of
//        it around)
// TODO - make easier callbacks (perhaps using makecontext?)

void resolver(void* old_promise) {

    // preamble //
    void* awaited_result = ((Promise*)old_promise)->value;
    scope_t* scope = ((Promise*)old_promise)->misc;
    // TODO - you can free the old promise now, I think...
    Generator* async_routine = scope->async_routine;
    Promise* master_promise = scope->master_promise;

    void* iteration_value = GeneratorNext(async_routine, awaited_result);

    if (iteration_value == NULL || async_routine->done) {
        PromiseResolve(master_promise, iteration_value);
    } else {
        Promise* awaited_promise = (Promise*)iteration_value;
        awaited_promise->misc = scope;
        PromiseThenSelf(awaited_promise, resolver);
    }

}

Promise* async(void (*func)(Generator*)) {
    Promise* p = PromiseCreate(events);
    Generator* async_routine = GeneratorMake(func);

    scope_t* scope = malloc(sizeof(scope_t));
    scope->async_routine = async_routine;
    scope->master_promise = p;
    p->misc = scope;

    resolver(p);
    return p;
}


// ================================= //


static void resolveRecord(void* prm) {
    resolve((Promise*)prm, NULL);
}

Promise* loadRecord() {
    Promise* p = new_Promise();
    setTimeout(resolveRecord, p, 250);
    return p;
}

void loadRecords(Generator* gen) {
    static int i = 0;
    for (i = 0; i < 2; i++) {
        yield (loadRecord());
        printf("loaded %d records...\n", i+1);
    }
    printf("done\n");
    yield_return (&i);
}

void main_finish(void* v_result) {
    printf("received records: %d\n", *(int*)v_result);
    stop_event_loop();
}

void jsmain(void* data) {
    then(async(loadRecords), main_finish);
}

int main() {
    start_event_loop(jsmain);
    return 0;
}
