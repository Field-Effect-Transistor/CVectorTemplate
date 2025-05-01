//  vector.c

#include "vector.h"

#ifdef DEBUG
    #include <stdio.h>
    #define DPRINT(...) printf(__VA_ARGS__)
#else
    #define DPRINT(...)
#endif

struct vector* initVector(
    void (*copy)(void** dst, const void* src),
    void (*destructor)(void*),
    size_t typeSize
) {
    if (true) {
        struct vector* v = malloc(sizeof(struct vector));
        if (!v) {
            DPRINT("Failed to allocate memory for vector\n");
            return NULL;
        }

        v->typeSize = typeSize ? typeSize : sizeof(void*);
        
        if (copy || destructor) {
            if (copy && destructor) {
                v->copy = copy;
                v->destructor = destructor;
            } else {
                DPRINT("You should provide both copy and destructor or neither\n");
                free(v);
                return NULL;
            }
        } else {
            v->copy = NULL;
            v->destructor = NULL;
        }

        v->data = (void**)malloc(INITIAL_CAPACITY * v->typeSize);
        if (!v->data) {
            DPRINT("Failed to allocate memory for vector data\n");
            free(v);
            return NULL;
        }

        v->size = 0;
        v->capacity = INITIAL_CAPACITY;
        return v;
    }

    return NULL;
}

void freeVector(struct vector* v) {
    if (v) {
        if (v->data) {
            if (v->destructor && v->size > 0) {
                char* ptr = (char*)v->data;
                char* end = ((char*)v->data) + v->size * v->typeSize;
                while (ptr < end) {
                    v->destructor(*(void**)ptr);
                    ptr += sizeof(void*);
                }
            }
            free(v->data);
        }
        free(v);
    }
}

bool pushBack(struct vector* v, const void* value) {
    if (v) {
        if (v->data) {
            if (!value) {
                DPRINT("Failed to push back via NULL ptr to value\n");
                return false;
            }

            if (v->size == v->capacity) {
                DPRINT("Reallocating memory for vector data\n");
                void** temp = (void**)malloc(2 * v->capacity * v->typeSize);
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
                v->copy((void**)((char*)v->data + v->size * v->typeSize), value);
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

bool insert(struct vector* v, size_t index, const void* value) {
    if (v) {
        if (v->data && index <= v->size) {
            if (v->size == v->capacity) {
                DPRINT("Reallocating memory for vector data\n");
                void** temp = (void**)malloc(2 * v->capacity * v->typeSize);
                if (!temp) {
                    DPRINT("Failed to reallocate memory for vector data\n");
                    return false;
                }

                memcpy(temp, v->data, index * v->typeSize);
                if (v->copy) {
                    v->copy((void**)((char*)temp + index * v->typeSize), value);
                } else {
                    memcpy((char*)temp + index * v->typeSize, value, v->typeSize);
                }
                memcpy(
                    (char*)temp + (index + 1) * v->typeSize, 
                    (char*)v->data + index * v->typeSize,
                    (v->size - index) * v->typeSize
                );

                v->capacity = 2 * v->capacity;
                free(v->data);
                v->data = temp;
            } else {
                memmove(
                    (char*)v->data + (index + 1) * v->typeSize,
                    (char*)v->data + index * v->typeSize,
                    (v->size - index) * v->typeSize
                );
                if (v->copy) {
                    v->copy((void**)((char*)v->data + index * v->typeSize), value);
                } else {
                    memcpy((char*)v->data + index * v->typeSize, value, v->typeSize);
                }
            }
            ++v->size;
            return true;
        }
    }

    DPRINT("Insert failed\n");
    return false;
}

void* at(struct vector* v, size_t index) {
    if (v) {
        if (v->data && index < v->size) {
            if (v->copy) {
                return *(void**)((char*)v->data + index * v->typeSize);
            }
            return (char*)v->data + index * v->typeSize;
        }
    }

    DPRINT("At failed\n");
    return NULL;
}

void* front(struct vector* v) {
    if (v) {
        if (v->data && v->size > 0) {
            if (v->copy) {
                return *v->data;
            }
            return v->data;
        }
    }

    DPRINT("Front failed\n");
    return NULL;
}

void* back(struct vector* v) {
    if (v) {
        if (v->data && v->size > 0) {
            if (v->copy) {
                return *(void**)((char*)v->data + (v->size - 1) * v->typeSize);
            }
            return (char*)v->data + (v->size - 1) * v->typeSize;
        }
    }

    DPRINT("Back failed\n");
    return NULL;
}

bool popBack(struct vector* v) {
    if (v) {
        if (v->data && v->size > 0) {
            --v->size;
            if (v->destructor) {
                v->destructor(*(void**)((char*)v->data + v->size * v->typeSize));
            }
            return true;
        }
    }

    DPRINT("Pop back failed\n");
    return false;
}

bool erase(struct vector* v, size_t index) {
    if (v) {
        if (v->data && index < v->size) {
            if (v->destructor) {
                v->destructor(*(void**)((char*)v->data + index * v->typeSize));
            }
            memmove(
                (char*)v->data + index * v->typeSize,
                (char*)v->data + (index + 1) * v->typeSize,
                (v->size - index - 1) * v->typeSize
            );
            --v->size;
            return true;
        }
    }

    DPRINT("Erase failed\n");
    return false;
}

void clear(struct vector* v) {
    if (v) {
        if (v->data && v->size > 0) {
            if (v->destructor) {
                char* ptr = (char*)v->data;
                char* end = ((char*)v->data) + v->size * v->typeSize;
                while (ptr < end) {
                    v->destructor(*(void**)ptr);
                    ptr += sizeof(void*);
                }
            }
            v->size = 0;
            return;
        }
    }

    DPRINT("Clear failed\n");
}

bool reserve(struct vector* v, size_t nCapacity) {
    if (v) {
        if (v->data) {
            if (nCapacity <= v->capacity) {
                DPRINT("Capacity is already enough\n");
                return true; 
            }

            void** temp = (void**)malloc(nCapacity * v->typeSize);
            if (!temp) {
                DPRINT("Failed to reallocate memory for vector data\n");
                return false;
            }

            memcpy(temp, v->data, v->size * v->typeSize);
            v->capacity = nCapacity;
            free(v->data);
            v->data = temp;
            return true;
        }
    }

    DPRINT("Reserve failed\n");
    return false;
}

bool isEmpty(struct vector* v) {
    return v->size == 0;
}

bool resize(struct vector* v, size_t nSize) {
    if (v) {
        if (v->data && nSize > 0) {
            if (nSize == v->size) {
                DPRINT("Size is already enough\n");
                return true;
            }

            if (nSize > v->size) {
                DPRINT("Cant resize vector to bigger size via danger of memory leak\n");
                return false;
            }

            if (v->destructor) {
                char* ptr = (char*)v->data + nSize * v->typeSize;
                char* end = ((char*)v->data) + v->size * v->typeSize;
                while (ptr < end) {
                    v->destructor(*(void**)ptr);
                    ptr += sizeof(void*);
                }
            }

            v->size = nSize;
            return true;

        }
    }

    DPRINT("Resize failed\n");
    return false;
}
