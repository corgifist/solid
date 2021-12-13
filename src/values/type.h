// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "../common.h"

typedef enum {
    INT
} ValueType;

typedef struct {
    ValueType type;
    union {
        long int_number;
        double float_number;
    } as;
} Value;

#define INT_VALUE(value) ((Value){INT, {.int_number = value}})

