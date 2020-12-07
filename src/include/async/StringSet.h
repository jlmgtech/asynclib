#ifndef string_set_h
#define string_set_h
#include <stdbool.h>

// a binary tree of strings

typedef struct StringSet {
    char** elements;
    size_t alloc_size;
    size_t count;
} StringSet;

StringSet* StringSetCreate();
void StringSetFinalize(void*);
void StringSetAdd(StringSet* this, char* element);
void StringSetRemove(StringSet* this, char* element);
bool StringSetHas(StringSet* this, char* element);

#endif
