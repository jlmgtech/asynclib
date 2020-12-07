#ifndef region_h
#define region_h
#include <stddef.h>

typedef struct region {
    size_t size;
    char* mem;
    size_t offs;
} region_t;

region_t* ropen(size_t size);
void* ralloc(region_t* this, size_t size);
void rclose(region_t* this);

#endif
