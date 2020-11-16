#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <ucontext.h>
#include <unistd.h>
#include <signal.h>
#include <stdarg.h>

typedef struct callback_t {
    void* caller;
    void** paramv;
    size_t paramc;
} callback_t;

callback_t* make_callback(void* caller) {
    callback_t* this = malloc(sizeof(callback_t));
    this->caller = caller;
    this->paramv = calloc(128, sizeof(void*));
    this->paramc = 0;
    return this;
}

void bind_param(callback_t* this, void* param) {
    printf("binding param %p\n", param);
    this->paramv[this->paramc++] = param;
}

void* call_callback(callback_t* this) {
    // for each param, push onto stack, then call, then return rax
    // https://stackoverflow.com/questions/50205294/portable-way-to-push-arguments-onto-the-call-stack-c
    void* retval;
    void* caller = this->caller;
    //for (size_t i = this->paramc; i > 0; i--) {
    //    void* param = this->paramv[i-1];
    //    printf("param: %p;\n", param);
    //    asm volatile ( "pushq %0;"
    //            :
    //            : "r"(param)
    //            );
    //}
    asm volatile( "pushq $1;\n"
            "pushq $2;\n"
            "subq $16, %%rsp;\n"
            "call *%0;\n"
            "movq %%rax, %1;\n"
            "addq $32, %%rsp;\n"
            : "=r"(retval)
            : "r"(caller)
            );
    return retval;
}

void* __attribute__((optimize("O0"))) dothing(void* a, void* b) {
    printf("dothing(%p, %p)\n", a, b);
    return 0x1337;
}

int main() {

    callback_t* cb = make_callback(dothing);
    bind_param(cb, (void*)2);
    bind_param(cb, (void*)3);

    void* result = call_callback(cb);
    printf("received result %p. \n", result);
    return 0;
}
