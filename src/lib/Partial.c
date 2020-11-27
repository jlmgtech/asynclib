#include <async/Generator.h>
#include <async/Partial.h>

void* PartialApply(partial_t* this, void* arg) {
    return GeneratorNext(this, arg);
}

partial_t* PartialCreate(void (func)(Generator*)) {
    partial_t* this = GeneratorCreate(func);
    PartialApply(this, NULL);
    return this;
}

void PartialDestroy(partial_t* this) {
    GeneratorDestroy(this);
}
