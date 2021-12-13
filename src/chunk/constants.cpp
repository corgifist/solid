// This script is part of SOLID language.
// Copyright 2021.

#include "constnants.h"
#include "allocations.h"

void initConstants(Constants* consts) {
    consts->count = 0;
    consts->capacity = 0;
    consts->values = NULL;
}

void writeConstants(Constants* consts, Value value) {
    if (consts->capacity < consts->count + 1) {
        int old = consts->capacity;
        consts->capacity = GROW_CAPACITY(old);
        consts->values = GROW_ARRAY(Value, consts->values, old, consts->capacity);
    }

    consts->values[consts->count] = value;
    consts->count++;
}

void freeConstants(Constants * consts) {
    FREE_ARRAY(Value , consts->values, consts->capacity);
    initConstants(consts);
}
