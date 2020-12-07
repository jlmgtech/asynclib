#include <stdio.h>
#define CL_POP(type, name) type name; cl_pop(_closure_data, sizeof(type), &name);
#define CL_RET(type, name) cl_push(_closure_data, sizeof(type), &name);
#define PUSHPAR(closure, type, name) cl_push(closure, sizeof(type), &name);
#define cl_free(x) free(x)

typedef struct closure_t {
    void (*function)(struct closure_t*);
    int _size;
    char* bp;
    int sp;
} closure_t;

closure_t* cl_create(void (*function)(closure_t*), const int stack_size);
void cl_push(closure_t* this, const int width, void* addr);
void cl_pop(closure_t* this, int width, void* addr);
void cl_call(closure_t* this);
