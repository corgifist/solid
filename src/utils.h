// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "values/type.h"
#include "common.h"

typedef enum {
    RETURN, CONSTANT, UNARY
} OpCode;

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#define IS_DIGIT(ch) ch >= '0' && ch <= '9'

#define SKIP_WHITESPACES() \
    case ' ': \
        break; \
    case '\r': \
        break; \
    case '\t': \
        break; \
    case '\n': \
        break

#define CHAR_PTR(ch) (reinterpret_cast<uint8_t *>(ch))

static std::string object_to_string(Value value) {
    switch (value.type) {
        case NUMBER:
            if (remainder(value.as.number, 2.0) == 0.0) {
                return SSTR((int) value.as.number);
            } else return SSTR(value.as.number);
        default: return "NULL";
    }
}

static size_t size(Value value) {
    switch (value.type) {
        case NUMBER:
            return sizeof(value.as.number);
        default: return 24;
    }
}
