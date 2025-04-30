//  vector.cpp

#include "vector.h"

#ifdef DEBUG
    #include <stdio.h>
    #define DPRINT(...) printf(__VA_ARGS__)
#else
    #define DPRINT(...)
#endif

struct vector* initVector(
    size_t typeSize,
    void*(*constructor)(void),
    void (*copy)(void* dst, const void* src),
    void (*destructor)(void*)
) {
    if (typeSize > 0) {
        struct vector* v = malloc(sizeof(struct vector));
        if (!v) {
        DPRINT("Failed to allocate memory for vector\n");
        return NULL;
        }

        v->data = malloc(INITIAL_CAPACITY * typeSize);
        if (!v->data) {
            DPRINT("Failed to allocate memory for vector data\n");
            free(v);
            return NULL;
        }

        v->size = 0;
        v->capacity = INITIAL_CAPACITY;
        v->typeSize = typeSize;
        v->constructor = constructor;
        v->destructor = destructor;
        v->copy = copy;
        return v;
    }

    return NULL;
}

void freeVector(struct vector* v) {
    if (v) {
        if (v->data) {
            if (v->destructor) {
                char* ptr = (char*)v->data;
                char* end = ((char*)v->data) + v->size * v->typeSize;
                while (ptr < end) {
                    v->destructor(ptr);
                    ptr += v->typeSize;
                }
            } else {
                free(v->data);
            }
        }
        free(v);
    }
}

bool pushBack(struct vector* v, void* value) {
    if (v) {
        if (v->data) {
            if (!value) {
                DPRINT("Value is NULL\n");
                return false;
            }

            if (v->size == v->capacity) {
                DPRINT("Reallocating memory for vector data\n");
                void* temp = malloc(2 * v->capacity * v->typeSize);
                if (!temp) {
                    DPRINT("Failed to reallocate memory for vector data\n");
                    return false;
                }
                memcpy(temp, v->data, v->size * v->typeSize);
                v->capacity = 2 * v->capacity;
                free(v->data);
                v->data = temp;
            }

            if (v->copy) {
                v->copy((char*)v->data + v->size * v->typeSize, value);
            } else {
                memcpy((char*)v->data + v->size * v->typeSize, value, v->typeSize);
            }
            ++v->size;
            return true;
        }
    }

    DPRINT("Push back failed\n");
    return false;
}

void* at(struct vector* v, size_t index) {
    if (v) {
        if (v->data && index < v->size) {
            return (char*)v->data + index * v->typeSize;
        }
    }

    DPRINT("At failed\n");
    return NULL;
}

void* front(struct vector* v) {
    if (v) {
        if (v->data && v->size > 0) {
            return v->data;
        }
    }

    DPRINT("Front failed\n");
    return NULL;
}

void* back(struct vector* v) {
    if (v) {
        if (v->data && v->size > 0) {
            return (char*)v->data + (v->size - 1) * v->typeSize;
        }
    }

    DPRINT("Back failed\n");
    return NULL;
}

bool insert(struct vector* v, size_t index, void* value) {
    if (v) {
        if (v->data && v->size < index) {
            if (!value) {
                DPRINT("Failed to insert via NULL ptr to value\n");
                return false;
            }

            if (v->size == v->capacity) {
                DPRINT("Realocating memory for vector data\n");
                void* temp = malloc(2 * v->capacity * v->typeSize);
                if (!temp) {
                    DPRINT("Failed to realocate memory for vector data\n");
                    return false;
                }

                memcpy(temp, v->data, index * v->typeSize);
                memcpy(
                    temp + (index + 2) * v->typeSize,
                    v->data + (index + 1) * v->typeSize,
                    v->typeSize * (v->size - index)
                );
                if (v->copy) {
                    v->copy(temp + (index + 1) * v->typeSize, value);
                } else {
                    memcpy(temp + (index + 1) * v->typeSize, value, v->typeSize);
                }

                v->capacity = 2 * v->capacity;
                free(v->data);
                v->data = temp;
            } else {
                memmove(
                    v->data + (index + 2) * v->typeSize,
                    v->data + (index + 1) * v->typeSize,
                    v->typeSize * (v->size - index)
                );
                if (v->copy) {
                    v->copy(v->data + (index + 1) * v->typeSize, value);
                } else {
                    memcpy(v->data + (index + 1) * v->typeSize, value, v->typeSize);
                }
            }
        }
    }

    DPRINT("Failed to insert\n");
    return false;
}