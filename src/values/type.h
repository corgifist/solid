// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "../common.h"

typedef enum {
    NUMBER
} ValueType;

typedef struct {
    ValueType type;
    union {
        double number;
    } as;
} Value;

#define NUMBER(value) ((Value){ NUMBER, {.number = value}})

#define IS_NUMBER(value) (value.type == NUMBER)

#define AS_NUMBER(value) (value.as.number)

#define IS_NUMBER_SUBSET(value) (value.type == NUMBER)

