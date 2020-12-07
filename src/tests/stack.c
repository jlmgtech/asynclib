#include <stdlib.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

typedef struct {
    size_t nmemb;
    size_t msize;
    long long si;
    char* data;
} stack_t;

stack_t* make_stack(size_t nmemb, size_t size) {
    stack_t* this = malloc(sizeof(stack_t) + size * nmemb);
    this->nmemb = nmemb;
    this->msize = size;
    this->si = 0;
    this->data = (char*)this + sizeof(stack_t);
    return this;
}

void stack_push(stack_t* this, void* src) {
    long long si = this->si;
    this->si += this->msize;
    void* dst = this->data + si;
    printf("%p = %p (%lu)\n", dst, src, this->msize);
    memcpy(dst, src, this->msize);
}

void stack_pop(stack_t* this, void* dst) {
    this->si -= this->msize;
    if (dst) {
        void* src = this->data + this->si;
        memcpy(dst, src, this->msize);
    }
}

stack_t* stack_clone(stack_t* this, size_t nmemb) {
    stack_t* that = malloc(sizeof(stack_t) + this->msize * nmemb);
    that->data = (char*)that + sizeof(stack_t);
    that->msize = this->msize;
    that->si = this->si;
    memcpy(that->data, this->data, this->msize * this->nmemb);
    return that;
}


typedef struct {
    char* name;
} person_t;

int main() {

    stack_t* people = make_stack(1024, sizeof(person_t));

    person_t jim;
    jim.name = "jimmy";

    person_t joe;
    joe.name = "joey";

    person_t who;

    printf("%p\n", (void*)people);
    stack_push(people, &jim);
    stack_push(people, &joe);
    stack_pop(people, &who);
    stack_pop(people, &who);

    printf("who is %s\n", who.name);
    return 0;
}


// all data structures that own their data:
//  stack - can hold values of a given type, can push, pop, get, and set
//  pipe - can hold values of a given type, can push, pull, get, and set
//  dynamic_stack - can hold values of any type, can push and pop
//  dynamic_pipe - can hold values of any type, can push and pull
