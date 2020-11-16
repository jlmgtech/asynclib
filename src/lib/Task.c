#include <stdlib.h>
#include "../include/Task.h"
#include "../include/Generator.h"

Task* TaskCreate(void* function, void* data) {
    Task* this = malloc(sizeof(Task));
    this->function = function;
    this->data = data;
    return this;
}

void TaskDestroy(Task* this) {
    if (is_generator(this->function)) {
        GeneratorFree(this->function);
    }
    free(this);
}

void TaskCall(Task* this) {
    if (is_generator(this->function)) {
        GeneratorNext((Generator*)this->function, this->data);
    } else {
        ((void (*)(void*))this->function)(this->data);
    }
}
