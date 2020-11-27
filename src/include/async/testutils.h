#include <async/Promise.h>

Events* events;
Emitter* emitter;
static bool _event_loop_running = true;

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

void start_event_loop(void (*entrypoint)(void*)) {
    events = EventsCreate();
    emitter = EmitterCreate();
    EventsPush(events, entrypoint, NULL);
    while (_event_loop_running) {
        EventsRun(events);
    }
    EmitterDestroy(emitter);
    EventsDestroy(events);
}

void stop_event_loop() {
    _event_loop_running = false;
}
