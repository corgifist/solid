// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "../chunk/allocations.h"

typedef enum {
    SHORT, INT, LONG, DOUBLE, STRING, BOOL, BYTE,
    // Unsigned types
    UNSIGNED_BYTE, UNSIGNED_SHORT, UNSIGNED_INT, UNSIGNED_LONG
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
        char byte;
        // Unsigned types
        unsigned char unbyte;
        unsigned short unshr;
        unsigned int unnt;
        unsigned long unlgn;
    } as;
} Value;


#define STRING(value) ((Value) {STRING, {.string = value}})
#define BYTE(value) ((Value) {BYTE, {.byte = value}})
#define SHORT(value) ((Value) {SHORT, {.shr = value}})
#define INT(value) ((Value) {INT, {.nt = value}})
#define LONG(value) ((Value) {LONG, {.lng = value}})
#define DOUBLE(value) ((Value) {DOUBLE, {.dbl = value}})
#define BOOL(value) ((Value) {BOOL, {.boolean = value}})

//Unsigned types

#define UNSIGNED_BYTE(value) ((Value) {UNSIGNED_BYTE, {.unbyte = value}})
#define UNSIGNED_SHORT(value) ((Value) {UNSIGNED_SHORT, {.unshr = value}})
#define UNSIGNED_INT(value) ((Value) {UNSIGNED_INT, {.unnt = value}})
#define UNSIGNED_LONG(value) ((Value) {UNSIGNED_LONG, {.unlgn = value}})

#define IS_NUMBER_SUBSET(value) (value.type == SHORT || value.type == INT || value.type == LONG || value.type == DOUBLE || value.type == BYTE) || \
    (value.type == UNSIGNED_BYTE || value.type == UNSIGNED_SHORT || value.type == UNSIGNED_INT || value.type == UNSIGNED_LONG)
