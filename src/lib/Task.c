#include <stdlib.h>
#include <stdio.h>
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
        printf("is generator\n");
        GeneratorNext((Generator*)this->function, this->data);
    } else {
        printf("is std function\n");
        ((void (*)(void*))this->function)(this->data);
    }
}
