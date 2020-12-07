#ifndef test_h
#define test_h

#include <string.h>
#include <stdlib.h>

#define test(x) \
    printf("\t%s - ", #x);\
    if (x()) {\
        printf(": PASS\n");\
    } else {\
        printf(": FAIL\n\n");\
        exit(1);\
    }

#define test_preamble() do { \
    char* fname = (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__); \
    printf("%s:\n", fname); \
} while(0)

#endif
