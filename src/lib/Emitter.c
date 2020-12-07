#include <stdio.h>
#include <stdlib.h>
#include <async/HashMap.h>
#include <async/Array.h>
#include <async/Emitter.h>
#include <async/rc.h>

Emitter* EmitterCreate() {
    Emitter* this = NEW(Emitter, EmitterFinalize, free);
    this->channels = HashMapCreate(1024);
    this->last_id = 0;
    return this;
}

typedef struct {
    size_t id;
    EventCallback callback;
    bool is_one_time;
} Handler;

Handler* HandlerCreate(size_t id, EventCallback callback, bool is_one_time) {
    Handler* this = malloc(sizeof(Handler));
    this->id = id;
    this->callback = callback;
    this->is_one_time = is_one_time;
    return this;
}

// TODO - DRY EmitterOnce and EmitterOn
size_t EmitterOnce(Emitter* this, char* topic, EventCallback callback) {
    Array* channel = HashMapGet(this->channels, topic);
    if (channel == NULL) {
        channel = ArrayCreate(1024);
        HashMapSet(this->channels, topic, channel);
    }
    size_t id = this->last_id++;
    Handler* handler = HandlerCreate(id, callback, true);
    ArrayPush(channel, (void*)handler);
    return id;
}
// TODO - DRY EmitterOnce and EmitterOn
size_t EmitterOn(Emitter* this, char* topic, EventCallback callback) {
    Array* channel = HashMapGet(this->channels, topic);
    if (channel == NULL) {
        channel = ArrayCreate(1024);
        HashMapSet(this->channels, topic, channel);
    }
    size_t id = this->last_id++;
    Handler* handler = HandlerCreate(id, callback, false);
    ArrayPush(channel, (void*)handler);
    return id;
}

void EmitterEmit(Emitter* this, char* topic, void* data) {
    Array* channel = HashMapGet(this->channels, topic);
    if (channel != NULL) {
        for (size_t i = 0; i < channel->count; i++) {
            Handler* handler = (Handler*)ArrayGet(channel, i);
            handler->callback(data);
            if (handler->is_one_time) {
                free(handler);
                ArraySet(channel, i, NULL);
            }
        }

        // clean up all nullified handlers
        for (long i = (long)channel->count-1; i >= 0; i--) {
            Handler* handler = (Handler*)ArrayGet(channel, i);
            if (handler == NULL) {
                ArrayRemove(channel, i);
            }
        }
    }
}

void EmitterRemoveListener(Emitter* this, char* topic, size_t id) {
    Array* channel = HashMapGet(this->channels, topic);
    if (channel == NULL) {
        return;
    }

    for (size_t i = 0; i < channel->count; i++) {
        Handler* handler = (Handler*)ArrayGet(channel, i);
        if (handler->id == id) {
            free(handler);
            ArrayRemove(channel, i);
            return;
        }
    }
}

void EmitterFinalize(void* ptr) {
    Emitter* this = (Emitter*)ptr;

    for (size_t i = 0; i < this->channels->keys->count; i++) {
        char* key = this->channels->keys->elements[i];
        Array* channel = HashMapGet(this->channels, key);
        for (int j = 0; j < channel->count; j++) {
            Handler* handler = (Handler*)ArrayGet(channel, j);
            free(handler);
        }
        DONE(channel);
    }
    DONE(this->channels);
}
