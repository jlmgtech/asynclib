#include <stdlib.h>
#include <stdio.h>
#include <async/Task.h>
#include <async/Generator.h>

Task* TaskCreate(void* function, void* data) {
    Task* this = malloc(sizeof(Task));
    this->function = function;
    this->data = data;
    return this;
}

void TaskDestroy(Task* this) {
    // don't free the generator, it confuses the programmer (aka myself)
    //if (is_generator(this->function)) { GeneratorDestroy(this->function) }
    free(this);
}

void TaskCall(Task* this) {
    if (is_generator(this->function)) {
        GeneratorNext((Generator*)this->function, this->data);
    } else {
        ((void (*)(void*))this->function)(this->data);
    }
}
