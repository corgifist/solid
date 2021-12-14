// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include<string>

typedef enum {
    SHORT, INT, LONG, DOUBLE, STRING
} ValueType;

typedef struct {
    ValueType type;
    union {
        const char *string;
        short shr;
        int nt;
        long lng;
        double dbl;
    } as;
} Value;

#define SHORT(value) ((Value) {SHORT, {.shr = value}})
#define INT(value) ((Value) {INT, {.nt = value}})
#define LONG(value) ((Value) {LONG, {.lng = value}})
#define DOUBLE(value) ((Value) {DOUBLE, {.dbl = value}})
#define STRING(value) ((Value) {STRING, {.string = value}})

#define IS_NUMBER(value) (value.type == NUMBER)
#define IS_STRING(value) (value.type == STRING)

#define AS_STRING(value) (value.as.string)

#define IS_NUMBER_SUBSET(value) (value.type == SHORT || value.type == INT || value.type == LONG || value.type == DOUBLE)

