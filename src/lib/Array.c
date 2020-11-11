#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../include/Array.h"

Array* ArrayCreate(size_t alloc_size) {
    if (alloc_size == 0) {
        alloc_size = 1024;
    }
    Array* this = malloc(sizeof(Array));
    this->elements = calloc(alloc_size, sizeof(char*));
    this->alloc_size = alloc_size;
    this->count = 0;
    return this;
}

void ArrayDestroy(Array* this) {
    free(this->elements);
    free(this);
}

static void expandElements(Array* this) {
    this->alloc_size *= 2;
    char** elements = calloc(this->alloc_size, sizeof(char*));
    memcpy(elements, this->elements, this->count * sizeof(char*));
    free(this->elements);
    this->elements = elements;
}

void ArrayPush(Array* this, char* element) {
    if (this->count >= this->alloc_size) {
        expandElements(this);
    }
    this->elements[this->count++] = element;
}

void ArrayUnshift(Array* this, char* element) {
    if (this->count >= this->alloc_size) {
        this->alloc_size *= 2;
    }
    char** elements = calloc(this->alloc_size, sizeof(char*));
    elements[0] = element;
    this->count++;
    for (size_t i = 1; i < this->count; i++) {
        elements[i] = this->elements[i-1];
    }
    free(this->elements);
    this->elements = elements;
}

void* ArrayPop(Array* this) {
    void* ptr = this->elements[--this->count];
    this->elements[this->count] = 0;
    return ptr;
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
