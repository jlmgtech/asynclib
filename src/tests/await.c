#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>
#include <time.h>
#include <sys/time.h>
#include <callback.h>
#include "../include/Events.h"
#include "../include/Emitter.h"
#include "../include/Promise.h"
#include "../include/Generator.h"

// TODO - tests, tests, and more tests
// TODO - part out the functions and structures that don't need to be in here
// TODO - async and await macros
// TODO - assume one universal event loop, and therefore simplify promises and event listeners
// TODO - simplify "above the hood" function signatures to make global assumptions about stuff
// TODO - collect the garbage - maybe you should just use gc_malloc...
// TODO - if not using a gc, use an RC or similar strategy for nullifying
//        promises at least (since you won't be expected to keep a reference of
//        it around)
// TODO - make easier callbacks (perhaps using makecontext?)

Events* events;
Emitter* emitter;
pthread_t timer;
bool keep_running = true;

typedef struct timeout_t {
    void (*callback)(void*);
    void* data;
    time_t end;
} timeout_t;

time_t now() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (time_t)(tv.tv_sec) * 1000 + (time_t)(tv.tv_usec) / 1000;
}

void _timeout(void* v_this) {
    timeout_t* this = (timeout_t*)v_this;
    if (this->end > now()) {
        EventsPush(events, _timeout, this);
    } else {
        this->callback(this->data);
        free(this);
    }
}

void setTimeout(void (*callback)(void*), void* data, time_t delay_ms) {
    timeout_t* this = malloc(sizeof(timeout_t));
    this->end = now() + delay_ms;
    this->data = data;
    this->callback = callback;
    EventsPush(events, _timeout, this);
}

size_t addEventListener(char* topic, void (*callback)(void*)) {
    return EmitterOn(emitter, topic, callback);
}

void removeListener(char* topic, size_t id) {
    return EmitterRemoveListener(emitter, topic, id);
}

typedef struct resolution_t {
    Generator* gen;
    void* val;
    Promise* prm;
} resolution_t;

void resolver(void* old_promise) {
    resolution_t* r = ((Promise*)old_promise)->misc;
    Promise* p = (Promise*)GeneratorNext(r->gen, r->val);
    if (p == NULL) {
        // generator did not return a promise, so we'll assume it's done?
        PromiseResolve(r->prm, NULL);
    } else if (r->gen->done == true) {
        // resolve the original promise with the return value from the
        // generator (if any)
        PromiseResolve(r->prm, r->gen->value);
    } else {
        p->misc = r;
        PromiseThenSelf(p, resolver);
    }
}

Promise* async(void (*func)(Generator*)) {
    Promise* p = PromiseCreate(events);
    Generator* g = GeneratorMake(func);

    resolution_t* rdata = malloc(sizeof(resolution_t));
    rdata->gen = g;
    rdata->prm = p;
    rdata->val = NULL;

    p->misc = rdata;
    resolver(p);
    return p;
}

void resolve(void* v_p) {
    Promise* p = (Promise*)v_p;
    PromiseResolve(p, NULL);
}

// ^ framework code ^
// ------------------
//   user code:

Promise* loadRecord() {
    Promise* p = PromiseCreate(events);
    setTimeout(resolve, p, 0);
    return p;
}

/* async */ void loadRecords(Generator* gen) {
    for (int i = 0; i < 2; i++) {
        /* await */ GeneratorYield(gen, loadRecord());
        printf("loaded %d records...\n", i+1);
    }
    printf("done\n");
    keep_running = false;
}

void main_finish(void* v_result) {
    printf("main_finish\n");
    //printf("received records: %d\n", *(int*)v_result);
}

void jsmain(void* data) {
    printf("setting timeout\n");
    Promise* p = async(loadRecords);
    PromiseThen(p, main_finish);
}

int main() {
    // create a thread for timeouts
    printf("started.\n");

    events = EventsCreate();
    emitter = EmitterCreate();
    EventsPush(events, jsmain, NULL);
    while (keep_running) {
        EventsRun(events);
    }
    EventsDestroy(events);
    EmitterDestroy(emitter);

    pthread_join(timer, NULL);
    printf("finished.\n");
    return 0;
}
