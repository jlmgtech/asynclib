#define _DEBUG
#include <async/region.h>
#include <async/test.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool test0() {
    printf("should allocate and deallocate a region");
    region_t* rd = ropen(1024);

    int* nums = ralloc(rd, sizeof(int) * 3);
    nums[0] = 0;
    nums[1] = 1;
    nums[2] = 2;
    int sum = nums[0] + nums[1] + nums[2];
    rclose(rd);

    return sum == 3;
}


bool test1() {
    printf("should allocate just enough for a couple integers");
    region_t* rd = ropen(sizeof(int)*2);
    int* num0 = ralloc(rd, sizeof(int));
    int* num1 = ralloc(rd, sizeof(int));
    *num0 = 100;
    *num1 = 200;
    int result = *num0 + *num1;
    rclose(rd);
    return result == 300;
}

int main() {
    test_preamble();
    test(test0);
    test(test1);
    return 0;
}
