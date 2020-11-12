typedef struct ReferenceCounter {
    void* data;
    void (*destructor)(void*);
    int count;
} ReferencCounter;

void ReferenceCounterInit(ReferenceCounter* this, void (*destructor)(void*)) {
    this->data = data;
    if (destructor == NULL) {
        this->destructor = free;
    } else {
        this->destructor = destructor;
    }
    this->count = 0;
}

void ReferenceCounterDestroy(ReferenceCounter* this) {
    this->destructor(this->data);
}

void ReferenceCounterCreate(void (*destructor)(void*)) {
    ReferenceCounter* this = malloc(sizeof(ReferenceCounter));
    ReferenceCounterInit(this, destructor);
    return this;
}

void ReferenceCounterInc(ReferenceCounter* this) {
    this->count++;
}

void ReferenceCounterDec(ReferenceCounter* this) {
    this->count--;
    if (this->count <= 0) {
        ReferenceCounterDestroy(this);
    }
}
