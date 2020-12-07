#include <async/closure.h>
#include <stdlib.h>
#include <memory.h>

closure_t* cl_create(void (*function)(closure_t*), const int stack_size) {
    closure_t* this = malloc(sizeof(closure_t) + stack_size);
    this->function = function;
    this->bp = (char*)this + sizeof(closure_t);
    this->sp = 0;
    this->_size = stack_size;
    return this;
}

void cl_push(closure_t* this, const int width, void* addr) {
    if (this->sp + width > this->_size) {
        fprintf(stderr, "ERROR: tried to push an argument onto a full stack\n");
        exit(1);
    }
    memcpy(this->bp + this->sp, addr, width);
    this->sp += width;
}

void cl_pop(closure_t* this, int width, void* addr) {
    this->sp -= width;
    if (this->sp < 0) {
        fprintf(stderr, "ERROR: tried to pop an argument off an empty closure stack\n");
        exit(1);
    }
    memcpy(addr, this->bp + this->sp, width);
}

void cl_call(closure_t* this) {
    this->function(this);
}
