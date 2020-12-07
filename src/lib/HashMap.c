#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <async/StringSet.h>
#include <async/HashMap.h>
#include <async/rc.h>


// TODO - make regions for buckets instead of relying on malloc
// to improve performance and mitigate fragmentation

static size_t hashify(char* str) {
    size_t hash = 0;
    while (*str) {
        hash += *str;
        str++;
    }
    return hash;
}

void* HashMapGet(HashMap* this, char* key) {
    size_t index = hashify(key) % this->num_buckets;
    HashMapValue* bucket = this->buckets[index];
    while (bucket != NULL) {
        if (strncmp(key, bucket->key, 1024) == 0) {
            return bucket->value;
        } else {
            bucket = bucket->next;
        }
    }
    return NULL;
}

void HashMapRemove(HashMap* this, char* key) {
    size_t index = hashify(key) % this->num_buckets;
    StringSetRemove(this->keys, key);
    HashMapValue* bucket = this->buckets[index];
    HashMapValue* previous = NULL;

    while (bucket != NULL) {
        if (strncmp(key, bucket->key, 1024) == 0) {
            if (previous == NULL) {
                // first in list
                this->buckets[index] = bucket->next;
            } else {
                // 2nd and 3rd and so on
                previous->next = bucket->next;
            }
            free(bucket);
            return;
        }
        previous = bucket;
        bucket = bucket->next;
    }
}

void HashMapSet(HashMap* this, char* key, void* value) {
    size_t index = hashify(key) % this->num_buckets;
    StringSetAdd(this->keys, key);
    if (this->buckets[index] == NULL) {
        this->buckets[index] = HashMapValueCreate(key, value);
    } else {
        HashMapValue* bucket = this->buckets[index];
        if (strcmp(key, bucket->key) == 0) {
            bucket->value = value;
            return;
        }
        while (bucket->next != NULL) {
            bucket = bucket->next;
            if (strcmp(key, bucket->key) == 0) {
                bucket->value = value;
                return;
            }
        }
        bucket->next = HashMapValueCreate(key, value);
    }
}

HashMap* HashMapCreate(size_t num_buckets) {
    HashMap* this = NEW(HashMap, HashMapFinalize, free);
    this->buckets = calloc(num_buckets, sizeof(HashMapValue));
    this->num_buckets = num_buckets;
    this->keys = StringSetCreate();
    return this;
}

void HashMapFinalize(void* ptr) {
    HashMap* this = (HashMap*)ptr;
    DONE(this->keys);
    for (int i = 0; i < this->num_buckets; i++) {
        if (this->buckets[i] != NULL) {
            HashMapValueDestroyList(this->buckets[i]);
        }
    }
    free(this->buckets);
}

HashMapValue* HashMapValueCreate(char* key, void* value) {
    HashMapValue* this = malloc(sizeof(HashMapValue));
    this->key = key;
    this->value = value;
    this->next = NULL;
    return this;
}

void HashMapValueDestroyList(HashMapValue* this) {
    if (this->next != NULL) {
        HashMapValueDestroyList(this->next);
    }
    free(this);
}
