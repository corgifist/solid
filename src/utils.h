// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "values/type.h"
#include "common.h"

typedef enum {
    RETURN, CONSTANT, UNARY, EXTRACT_BIND, BINARY
} OpCode;

#define SSTR(x) static_cast< std::ostringstream & >( \
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

static double EXACT_OPERAND(Value operand) {
    switch (operand.type) {
        case SHORT:
            return (double) operand.as.shr;
        case INT:
            return (double) operand.as.nt;
        case LONG:
            return (double) operand.as.lng;
        case DOUBLE:
            return operand.as.dbl;
    }
    return 0;
}

static Value NUMBER_FROM_SOURCE(Value source, double operand) {
    switch (source.type) {
        case SHORT:
            return SHORT((short) operand);
        case INT:
            return INT((int) operand);
        case LONG:
            return LONG((long) operand);
        case DOUBLE:
        default:
            return DOUBLE(operand);
    }
}

static std::string object_to_string(Value value) {
    if (IS_NUMBER_SUBSET(value)) {
        switch (value.type) {
            case SHORT:
                return SSTR(value.as.shr);
            case INT:
                return SSTR(value.as.nt);
            case LONG:
                return SSTR(value.as.lng);
            case DOUBLE:
                return SSTR(value.as.dbl);
        }
    }
    switch (value.type) {
        case STRING:
            return value.as.string;
        default:
            return "NULL";
    }
}

static size_t size(Value value) {
    switch (value.type) {
        case SHORT:
        case INT:
        case LONG:
        case DOUBLE:
            return 24;
        case STRING:
            return sizeof(value.as.string);
        default:
            return 8;
    }
}

static const char* type(Value operand) {
    switch (operand.type) {
        case SHORT:
            return "short";
        case INT:
            return "int";
        case LONG:
            return "long";
        case DOUBLE:
            return "double";
        case STRING:
            return "string";
    }
}
