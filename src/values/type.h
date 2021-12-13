// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include<string>

typedef enum {
    NUMBER, STRING
} ValueType;

typedef struct {
    ValueType type;
    union {
        const char* string;
        double number;
    } as;
} Value;

#define NUMBER(value) ((Value){ NUMBER, {.number = value}})
#define STRING(value) ((Value) {STRING, {.string = value}})

#define IS_NUMBER(value) (value.type == NUMBER)
#define IS_STRING(value) (value.type == STRING)

#define AS_NUMBER(value) (value.as.number)
#define AS_STRING(value) (value.as.string)

#define IS_NUMBER_SUBSET(value) (value.type == NUMBER)

