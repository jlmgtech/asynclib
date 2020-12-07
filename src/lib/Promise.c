#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <async/rc.h>
#include <async/Promise.h>

static void add_listener(Promise* this, PromiseCallback callback, bool pass_self) {
    PromiseNode* listener = calloc(1, sizeof(PromiseNode));
    listener->callback = callback;
    listener->pass_self = pass_self;
    if (this->listeners == NULL) {
        this->listeners = listener;
    } else {
        this->listeners->prev = listener;
        listener->next = this->listeners;
        this->listeners = listener;
    }
}

static void schedule_listeners(Promise* this) {
    PromiseNode* listener = this->listeners;
    while (listener != NULL) {
        PromiseNode* current = listener;
        if (listener->pass_self) {
            EventsPush(this->events, listener->callback, this);
        } else {
            EventsPush(this->events, listener->callback, this->value);
        }
        listener = current->next;
        free(current);
    }
    this->listeners = NULL;
}


Promise* PromiseCreate(Events* events) {
    Promise* this = NEW(Promise, PromiseFinalize, free);
    PromiseInit(this, events);
    return this;
}

void PromiseInit(Promise* this, Events* events) {
    this->events = events;
    this->value = NULL;
    this->is_resolved = false;
    this->listeners = NULL;
    this->__misc = NULL;
}

void PromiseFinalize(void* ptr) {
    Promise* this = (Promise*)ptr;
    while (this->listeners != NULL) {
        PromiseNode* listener = this->listeners->next;
        free(this->listeners);
        this->listeners = listener;
    }
}

void PromiseResolve(Promise* this, void* data) {
    // can only resolve once!
    if (this->is_resolved == false) {
        this->is_resolved = true;
        this->value = data;
        schedule_listeners(this);
    }
}

void _PromiseThen(Promise* this, PromiseCallback callback, bool pass_self) {
    add_listener(this, callback, pass_self);
    if (this->is_resolved == true) {
        schedule_listeners(this);
    }
}

void PromiseThen(Promise* this, PromiseCallback callback) {
    _PromiseThen(this, callback, false);
}

void PromiseThenSelf(Promise* this, PromiseCallback callback) {
    _PromiseThen(this, callback, true);
}

