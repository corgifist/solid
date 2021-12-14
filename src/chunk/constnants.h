// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "../values/type.h"

typedef struct {
    int count, capacity;
    Value *values;
} Constants;

void initConstants(Constants *consts);

void writeConstants(Constants *consts, Value value);

void freeConstants(Constants *consts);
