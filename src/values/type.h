// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "../chunk/allocations.h"

typedef enum {
    SHORT, INT, LONG, DOUBLE, STRING, BOOL, BYTE
} ValueType;

typedef struct {
    ValueType type;
    union {
        short shr;
        int nt;
        long lng;
        double dbl;
        const char* string;
        bool boolean;
        unsigned char byte;
    } as;
} Value;


#define STRING(value) ((Value) {STRING, {.string = value}})
#define BYTE(value) ((Value) {BYTE, {.byte = value}})
#define SHORT(value) ((Value) {SHORT, {.shr = value}})
#define INT(value) ((Value) {INT, {.nt = value}})
#define LONG(value) ((Value) {LONG, {.lng = value}})
#define DOUBLE(value) ((Value) {DOUBLE, {.dbl = value}})
#define BOOL(value) ((Value) {BOOL, {.boolean = value}})

#define IS_NUMBER_SUBSET(value) (value.type == SHORT || value.type == INT || value.type == LONG || value.type == DOUBLE || value.type == BYTE)

