#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../include/StringSet.h"

StringSet* StringSetCreate() {
    StringSet* this = malloc(sizeof(StringSet));
    this->elements = calloc(1024, sizeof(char*));
    this->alloc_size = 1024;
    this->count = 0;
    return this;
}

void StringSetDestroy(StringSet* this) {
    free(this->elements);
    free(this);
}

static void expandElements(StringSet* this) {
    this->alloc_size *= 2;
    char** elements = calloc(this->alloc_size, sizeof(char*));
    memcpy(elements, this->elements, this->count * sizeof(char*));
    free(this->elements);
    this->elements = elements;
}

void StringSetAdd(StringSet* this, char* element) {
    if (!StringSetHas(this, element)) {
        if (this->count >= this->alloc_size-1) {
            expandElements(this);
        }
        this->elements[this->count++] = element;
    }
}

void StringSetRemove(StringSet* this, char* element) {
    // basically, this.elements = this.elements.filter(...)
    char** elements = calloc(this->alloc_size, sizeof(char*));
    for (size_t i = 0; i < this->count; i++) {
        if (strcmp(this->elements[i], element) != 0) {
            elements[i] = this->elements[i];
        }
    }
    free(this->elements);
    this->elements = elements;
    this->count--;
}

bool StringSetHas(StringSet* this, char* element) {
    for (size_t i = 0; i < this->count; i++) {
        if (strcmp(element, this->elements[i]) == 0) {
            return true;
        }
    }
    return false;
}
