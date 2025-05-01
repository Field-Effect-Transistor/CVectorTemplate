//  test.c

#include <stdio.h>
#include "vector.h"

struct test {
    int a;
    int b;
};

void copyTest(void** dst, const void* src) {
    *dst = malloc(sizeof(struct test));
    struct test* dstTest = (struct test*)(*dst);
    struct test* srcTest = (struct test*)src;
    dstTest->a = srcTest->a;
    dstTest->b = srcTest->b;
};

void deleteTest(void* test) {
    free(test);
}

int main(void) {
    struct vector* v = initVector(NULL, NULL, sizeof(int));
    struct vector* v2 = initVector(copyTest, deleteTest, 0);

    reserve(v, 3);
    reserve(v2, 5);

    for (size_t i = 0; i < 10; ++i) {
        struct test t = { .a = i, .b = i + 1 };
        pushBack(v, &i);
        pushBack(v2, &t);
    }

    printf("V:\n");
    for (size_t i = 0; i < v->size; ++i) {
        printf("v[%d]=%2d\n ", i, *(int*)at(v, i));
    }
    printf("\nV2:\n");
    for (size_t i = 0; i < v2->size; ++i) {
        struct test* t = (struct test*)at(v2, i);
        printf("v2[%d]=%2d:%2d\n", i, t->a, t->b);
    }
    printf("\n");

    struct test temp = { .a = 0, .b = 0 };

    insert(v, 0, &(int){0}) ? printf("Inserted\n") : printf("Failed to insert\n");
    insert(v, v->size / 2, &(int){0}) ? printf("Inserted\n") : printf("Failed to insert\n");
    //insert(v, v->size, &(int){0}) ? printf("Inserted\n") : printf("Failed to insert\n");
    popBack(v) ? printf("Popped\n") : printf("Failed to pop\n");
    erase(v, v->size / 2) ? printf("Erased %i-element\n", v->size / 2) : printf("Failed to erase\n");
    
    insert(v2, 0, &temp)? printf("Inserted\n") : printf("Failed to insert\n");
    insert(v2, v2->size / 2, &temp) ? printf("Inserted\n") : printf("Failed to insert\n");
    //insert(v2, v2->size, &temp) ? printf("Inserted\n") : printf("Failed to insert\n");
    popBack(v2) ? printf("Popped\n") : printf("Failed to pop\n");
    erase(v2, v2->size / 2 + 1) ? printf("Erased %i-element\n", v2->size / 2 + 1) : printf("Failed to erase\n");

    printf("V:\n");
    for (size_t i = 0; i < v->size; ++i) {
        printf("v[%d]=%2d\n ", i, *(int*)at(v, i));
    }
    printf("\nV2:\n");
    for (size_t i = 0; i < v2->size; ++i) {
        struct test* t = (struct test*)at(v2, i);
        printf("v2[%d]=%2d:%2d\n", i, t->a, t->b);
    }
    printf("\n");

    resize(v, v->size / 2) ? printf("Resized\n") : printf("Failed to resize\n");
    resize(v2, v2->size / 2) ? printf("Resized\n") : printf("Failed to resize\n");

    printf("V:\n");
    for (size_t i = 0; i < v->size; ++i) {
        printf("v[%d]=%2d\n ", i, *(int*)at(v, i));
    }
    printf("\nV2:\n");
    for (size_t i = 0; i < v2->size; ++i) {
        struct test* t = (struct test*)at(v2, i);
        printf("v2[%d]=%2d:%2d\n", i, t->a, t->b);
    }
    printf("\n");

    clear(v);
    clear(v2);
    printf("V capacity: %d\nV2 capacity: %d\n", v->capacity, v2->capacity);
    printf("V size: %d\nV2 size: %d\n", v->size, v2->size);

    freeVector(v);
    freeVector(v2);
    return 0;
}
