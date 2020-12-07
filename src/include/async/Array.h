#ifndef array_h
#define array_h
#include <stdbool.h>
#include <stddef.h>

typedef struct Array {
    size_t alloc_size;
    size_t count;
    void** elements;
} Array;

Array* ArrayCreate(size_t size);
void ArrayFinalize(void*);

void ArrayPush(Array* this, void* element);
void ArrayUnshift(Array* this, void* element);

void* ArrayGet(Array* this, size_t index);
bool ArraySet(Array* this, size_t index, void* element);

void* ArrayPop(Array* this);
void* ArrayShift(Array* this);
void* ArrayRemove(Array* this, size_t index);

#endif
