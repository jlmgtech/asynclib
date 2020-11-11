#ifndef emitter_h
#define emitter_h
#include <stddef.h>
#include "./HashMap.h"

typedef void (*EventCallback)(void*);

typedef struct Emitter {
    HashMap* channels;
    size_t last_id;
} Emitter;

Emitter* EmitterCreate();
size_t EmitterOn(Emitter* this, char* topic, EventCallback callback);
size_t EmitterOnce(Emitter* this, char* topic, EventCallback callback);
void EmitterEmit(Emitter* this, char* topic, void* data);
void EmitterRemoveListener(Emitter* this, char* topic, size_t id);
void EmitterDestroy(Emitter* this);

#endif
