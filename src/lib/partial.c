#include "../include/partial.h"

void* apply_partial(partial_t* this, void* arg) {
    return GeneratorNext(this, arg);
}

partial_t* make_partial(void (func)(Generator*)) {
    partial_t* this = GeneratorMake(func);
    apply_partial(this, NULL);
    return this;
}

void free_partial(partial_t* this) {
    GeneratorFree(this);
}
