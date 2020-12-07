#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <async/closure.h>
#include <async/test.h>

typedef struct person {
    int id;
    char* fname;
    char* lname;
} person_t;

void callme(closure_t* _closure_data) {
    CL_POP(person_t, jambo);
    CL_POP(person_t, jimbo);
    //printf("jimbo: %d: %s %s\n", jimbo.id, jimbo.fname, jimbo.lname);
    //printf("jambo: %d: %s %s\n", jambo.id, jambo.fname, jambo.lname);
    int checksum = jimbo.id - jambo.id;
    CL_RET(int, checksum);
}

bool test0() {
    printf("should call a closure with two structures and return an int");

    int retval;
    person_t jambo;
    person_t jimbo;
    closure_t* callme_c;

    jambo.fname = "jambo";
    jambo.lname = "mambo";
    jambo.id = 1235;

    jimbo.fname = "james";
    jimbo.lname = "lay";
    jimbo.id = 1234;

    callme_c = cl_create(callme, sizeof(person_t)*2);
    cl_push(callme_c, sizeof(person_t), &jimbo);
    cl_push(callme_c, sizeof(person_t), &jambo);
    cl_call(callme_c);
    cl_pop(callme_c, sizeof(int), &retval);
    cl_free(callme_c);

    return retval == jimbo.id - jambo.id;
}

bool test1() {
    printf("should call a closure that takes a structure, an int, and a string");
}

int main() {
    test_preamble();
    test(test0);
    return 0;
}
