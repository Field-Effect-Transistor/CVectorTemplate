//  test.c

#include <stdio.h>
#include "vector.h"

struct test {
    int a;
    int b;
};

void copyTest(void* dst, const void* src) {
    struct test* dstTest = (struct test*)dst;
    struct test* srcTest = (struct test*)src;
    dstTest->a = srcTest->a;
    dstTest->b = srcTest->b;
};

void deleteTest(void* test) {
    free(test);
}

int main(void) {
    struct vector* v = initVector(sizeof(int), NULL, NULL, NULL);
    struct vector* v2 = initVector(sizeof(struct test), NULL, copyTest, deleteTest);

    for (int i = 0; i < 10; ++i) {
        printf("%d\n", pushBack(v, &i));
        printf("%d\n", ((int*)v->data)[i]);
        printf("%ld:%ld\n\n", v->size, v->capacity);
    }

    for (int i = 0; i < 15; ++i) {
        printf("====");
    }
    printf("\n");

    for (int i = 0; i < 8; ++i) {
        struct test t = {i, i - 1};
        printf("%d\n", pushBack(v2, &t));
        printf("%d:%d\n", ((struct test*)v2->data)[i].a, ((struct test*)v2->data)[i].b);
        printf("%ld:%ld\n\n", v2->size, v2->capacity);
    }

    int* a = (int*)malloc(sizeof(int) * 2);

    freeVector(v);
    freeVector(v2);
    return 0;
}