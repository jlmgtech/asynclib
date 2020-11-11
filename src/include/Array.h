#ifndef array_h
#define array_h
#include <stdbool.h>
#include <stddef.h>

typedef struct Array {
    char** elements;
    size_t alloc_size;
    size_t count;
} Array;

Array* ArrayCreate(size_t size);
void ArrayDestroy(Array* this);

void ArrayPush(Array* this, char* element);
void ArrayUnshift(Array* this, char* element);

void* ArrayGet(Array* this, size_t index);
bool ArraySet(Array* this, size_t index, void* element);

void* ArrayPop(Array* this);
void* ArrayShift(Array* this);

#endif
