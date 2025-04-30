//  vector.h

#pragma once

#include <stdlib.h>
#include <stdbool.h>    
#include <string.h>

static const size_t INITIAL_CAPACITY = 2;

struct vector {
    void** data;
    size_t size;
    size_t capacity;

    size_t typeSize;    //  only for POD types | for non-POD types use sizeof(void*) or 0

    //  only for non-POD types
    //  both constructor and destructor should allocate and free memory for structure
    //  if common constructor or destructor doesn't do it, you should write your own
    void (*copy)(void** dst, const void* src); //  should allocate memory for structure
    void (*destructor)(void*);          //  should free memory allocated for structure
};

struct vector* initVector(
    void (*copy)(void** dst, const void* src),
    void (*destructor)(void*),
    size_t typeSize
);
void freeVector(struct vector* v);

bool pushBack(struct vector* v, void* value);
bool insert(struct vector* v, size_t index, void* value);

bool popBack(struct vector* v);
bool erase(struct vector* v, size_t index);
void clear(struct vector* v);

void* at(struct vector* v, size_t index);
void* front(struct vector* v);
void* back(struct vector* v);

bool resize(struct vector* v, size_t nSize);
bool reserve(struct vector* v, size_t nCapacity);
