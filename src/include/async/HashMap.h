#ifndef map_h
#define map_h
#include "./StringSet.h"

typedef struct HashMapValue {
    char* key;
    void* value;
    struct HashMapValue* next;
} HashMapValue;
HashMapValue* HashMapValueCreate(char* key, void* value);
void HashMapValueDestroyList(HashMapValue* this);


// ---------------------------- //

typedef struct HashMap {
    HashMapValue** buckets;
    size_t num_buckets;
    StringSet* keys;
} HashMap;

HashMap* HashMapCreate(size_t num_buckets);
void HashMapDestroy(HashMap* this);
void* HashMapGet(HashMap* this, char* key);
void HashMapSet(HashMap* this, char* key, void* value);
void HashMapRemove(HashMap* this, char* key);

#endif
