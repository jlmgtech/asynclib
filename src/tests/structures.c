#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <async/HashMap.h>
#include <async/Array.h>
#include <async/StringSet.h>
#include <async/test.h>
#include <async/rc.h>

bool test0() {
    printf("should return the value that was set on key 'a'");
    bool success;
    int value = 10;
    HashMap* map = HashMapCreate(1024);
    HashMapSet(map, "aaaa", (void*)&value);
    int *newval = (int*)HashMapGet(map, "aaaa");
    if (newval == NULL) {
        success = false;
    } else {
        success = value == *newval;
    }
    DONE(map);
    return success;
}

bool test1() {
    printf("should return NULL when trying to access a key that hasn't been set");
    bool success;
    HashMap* map = HashMapCreate(1024);
    int value = 10;
    HashMapSet(map, "a", (void*)&value);
    int *newval = (int*)HashMapGet(map, "bbb");
    success = newval == NULL;
    DONE(map);
    return success;
}

bool test1_2() {
    printf("should test resetting a key");
    bool success;
    HashMap* map = HashMapCreate(1024);
    int value_a = 10;
    int value_b = 11;
    HashMapSet(map, "a", (void*)&value_a);
    HashMapSet(map, "a", (void*)&value_b);
    int *newval = (int*)HashMapGet(map, "a");
    success = *newval == 11;
    DONE(map);
    return success;
}

bool test1_3() {
    printf("should test resetting a deleted key");
    bool success;
    HashMap* map = HashMapCreate(1024);
    int value_a = 10;
    int value_b = 11;
    HashMapSet(map, "a", (void*)&value_a);
    HashMapRemove(map, "a");
    HashMapSet(map, "a", (void*)&value_b);
    int *newval = (int*)HashMapGet(map, "a");
    success = *newval == 11;
    DONE(map);
    return success;
}

bool test1_4() {
    printf("should test that a key is removed when it has no more listeners");
    HashMap* map = HashMapCreate(1024);
    int value_a = 10;
    int value_b = 11;
    HashMapSet(map, "a", (void*)&value_a);
    HashMapRemove(map, "a");
    bool success = map->keys->count == 0 &&
        map->keys->elements[0] == NULL;
    DONE(map);
    return success;
}

bool test2() {
    printf("should handle collisions");
    bool success;
    HashMap* map = HashMapCreate(1);
    int aval = 10;
    int bval = 11;
    HashMapSet(map, "jimmy", (void*)&aval);
    HashMapSet(map, "Bob brown Jr.", (void*)&bval);

    int* aval_2 = (int*)HashMapGet(map, "jimmy");
    int* bval_2 = (int*)HashMapGet(map, "Bob brown Jr.");

    success = *aval_2 == aval && *bval_2 == bval;
    DONE(map);
    return success;
}

bool test2_2() {
    printf("should not detect a key that was removed");

    int aval = 10;

    HashMap* map = HashMapCreate(10);
    HashMapSet(map, "a", &aval);
    HashMapRemove(map, "a");
    bool success = HashMapGet(map, "a") == NULL;
    DONE(map);

    return success;
}

bool test2_3() {
    printf("should handle case of removing first collided key");
    bool success;

    int aval = 10;
    int bval = 11;
    int cval = 12;

    HashMap* map = HashMapCreate(1);

    // all three collide
    HashMapSet(map, "a", &aval);
    HashMapSet(map, "b", &bval);
    HashMapSet(map, "c", &cval);

    HashMapRemove(map, "a");
    success = HashMapGet(map, "a") == NULL &&
        HashMapGet(map, "b") != NULL &&
        HashMapGet(map, "c") != NULL;

    DONE(map);

    return success;
}

bool test2_4() {
    printf("should handle case of removing middle collided key");
    bool success;

    int aval = 10;
    int bval = 11;
    int cval = 12;

    HashMap* map = HashMapCreate(1);

    // all three collide
    HashMapSet(map, "a", &aval);
    HashMapSet(map, "b", &bval);
    HashMapSet(map, "c", &cval);

    HashMapRemove(map, "b");
    success = HashMapGet(map, "a") != NULL &&
        HashMapGet(map, "b") == NULL &&
        HashMapGet(map, "c") != NULL;

    DONE(map);

    return success;
}

bool test2_5() {
    printf("should handle case of removing final collided key");
    bool success;

    int aval = 10;
    int bval = 11;
    int cval = 12;

    HashMap* map = HashMapCreate(1);

    // all three collide
    HashMapSet(map, "a", &aval);
    HashMapSet(map, "b", &bval);
    HashMapSet(map, "c", &cval);

    HashMapRemove(map, "c");
    success = HashMapGet(map, "a") != NULL &&
        HashMapGet(map, "b") != NULL &&
        HashMapGet(map, "c") == NULL;

    DONE(map);

    return success;
}

bool test3() {
    printf("should detect an element added to the set");
    StringSet* set = StringSetCreate();
    StringSetAdd(set, "hi");
    StringSetAdd(set, "hi2");
    bool success = StringSetHas(set, "hi") &&
        StringSetHas(set, "hi2") &&
        !StringSetHas(set, "there");
    DONE(set);
    return success;
}

bool test4() {
    printf("should not detect an element removed from the set");
    StringSet* set = StringSetCreate();
    StringSetAdd(set, "hi");
    StringSetAdd(set, "there");
    StringSetRemove(set, "there");
    bool success = StringSetHas(set, "hi") &&
        !StringSetHas(set, "there");
    DONE(set);
    return success;
}

bool test5() {
    printf("should be able to properly count number of elements");
    StringSet* set = StringSetCreate();
    StringSetAdd(set, "0");
    StringSetAdd(set, "1");
    StringSetAdd(set, "2");
    StringSetAdd(set, "3");
    StringSetAdd(set, "4");
    bool success = set->count == 5;
    DONE(set);
    return success;
}

static char *rand_string(char *str, size_t size) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJK...";
    if (size) {
        --size;
        for (size_t n = 0; n < size; n++) {
            int key = rand() % (int) (sizeof charset - 1);
            str[n] = charset[key];
        }
        str[size] = '\0';
    }
    return str;
}

char* rand_string_alloc(size_t size) {
    char *s = malloc((size + 1) * sizeof(char));
    if (s) {
        rand_string(s, size);
    }
    return s;
}

bool test6() {
    printf("should be able to hold more than 1024 elements");
    StringSet* set = StringSetCreate();
    for (int i = 0; i < 2048; i++) {
        char* string = rand_string_alloc(10);
        StringSetAdd(set, string);
    }
    StringSetAdd(set, "hi there");
    bool success = set->count == 2049 && StringSetHas(set, "hi there");
    for (int i = 0; i < 2048; i++) {
        free(set->elements[i]);
    }
    DONE(set);
    return success;
}

bool test7() {
    printf("should pop the pushed element");
    Array* arr = ArrayCreate(0);
    ArrayPush(arr, "hi");
    char* result = (char*)ArrayPop(arr);
    bool success = strcmp(result, "hi") == 0;
    DONE(arr);
    return success;
}

bool test8() {
    printf("should shift the unshifted element");
    Array* arr = ArrayCreate(0);
    ArrayUnshift(arr, "hi");
    char* result = (char*)ArrayShift(arr);
    bool success = strcmp(result, "hi") == 0;
    DONE(arr);
    return success;
}

bool test9() {
    printf("should pop the popped and shift the unshifted");

    Array* arr = ArrayCreate(0);
    ArrayPush(arr, "pushed");
    ArrayUnshift(arr, "unshifted");

    char* pushed = (char*)ArrayPop(arr);
    char* unshifted = (char*)ArrayShift(arr);

    bool success = strcmp(pushed, "pushed") == 0 &&
        strcmp(unshifted, "unshifted") == 0;

    DONE(arr);
    return success;
}

bool test10() {
    printf("should get and set elements");
    Array* arr = ArrayCreate(0);
    ArrayPush(arr, "a");
    ArrayPush(arr, "b");
    ArrayPush(arr, "c");
    ArrayPush(arr, "d");

    bool success = strcmp("b", (char*)ArrayGet(arr, 1)) == 0;
    ArraySet(arr, 1, "z");
    success = success && strcmp("z", ArrayGet(arr, 1)) == 0;

    DONE(arr);
    return success;
}

int main() {
    test_preamble();

    // hash map tests
    test(test0);
    test(test1);
    test(test1_2);
    test(test1_3);
    test(test1_4);
    test(test2);
    test(test2_2);
    test(test2_3);
    test(test2_4);
    test(test2_5);

    // string set tests
    test(test3);
    test(test4);
    test(test5);
    test(test6);

    // array tests
    test(test7);
    test(test8);
    test(test9);
    test(test10);

    return 0;
}

