// This script is part of SOLID language.
// Copyright 2021.

#include "allocations.h"

void *reallocate(void *pointer, size_t oldSize, size_t newSize) {
    if (newSize == 0) {
        free(pointer);
        return NULL;
    }

    void *result = realloc(pointer, newSize);
    if (result == NULL) system_exception("AllocationFailure", "out of memory");
    return result;
}

