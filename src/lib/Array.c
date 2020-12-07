#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <async/Array.h>
#include <async/rc.h>

Array* ArrayCreate(size_t alloc_size) {
    if (alloc_size == 0) {
        alloc_size = 1024;
    }
    Array* this = NEW(Array, ArrayFinalize, free);
    this->elements = calloc(alloc_size, sizeof(void*));
    this->alloc_size = alloc_size;
    this->count = 0;
    return this;
}

void ArrayFinalize(void* ptr) {
    free(((Array*)ptr)->elements);
}

void* ArrayRemove(Array* this, size_t index) {
    if (index >= this->count) {
        return NULL;
    }
    if (index == 0) {
        return ArrayShift(this);
    }
    if (index == this->count-1) {
        return ArrayPop(this);
    }

    void* ptr = ArrayGet(this, index);
    this->count--;
    for (size_t i = index; i < this->count; i++) {
        this->elements[i] = this->elements[i+1];
    }
    return ptr;
}

static void expandElements(Array* this) {
    this->alloc_size *= 2;
    void** elements = calloc(this->alloc_size, sizeof(void*));
    memcpy(elements, this->elements, this->count * sizeof(void*));
    free(this->elements);
    this->elements = elements;
}

void ArrayPush(Array* this, void* element) {
    if (this->count >= this->alloc_size) {
        expandElements(this);
    }
    this->elements[this->count++] = element;
}

void* ArrayPop(Array* this) {
    void* ptr = this->elements[--this->count];
    this->elements[this->count] = 0;
    return ptr;
}

void* ArrayShift(Array* this) {
    if (this->count > 0) {
        void* element = this->elements[0];
        if (this->count == 1) {
            this->elements[0] = NULL;
        } else {
            for (size_t i = 0; i < this->count; i++) {
                this->elements[i] = this->elements[i+1];
            }
        }
        this->count--;
        return element;
    }
    return NULL;
}

void ArrayUnshift(Array* this, void* element) {
    if (this->count >= this->alloc_size) {
        this->alloc_size *= 2;
    }
    void** elements = calloc(this->alloc_size, sizeof(void*));
    elements[0] = element;
    this->count++;
    for (size_t i = 1; i < this->count; i++) {
        elements[i] = this->elements[i-1];
    }
    free(this->elements);
    this->elements = elements;
}

bool ArraySet(Array* this, size_t index, void* element) {
    if (this->count > index) {
        this->elements[index] = element;
        return true;
    }
    return false;
}

void* ArrayGet(Array* this, size_t index) {
    if (this->count > index) {
        return this->elements[index];
    } else {
        return NULL;
    }
}

