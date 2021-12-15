// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "values/type.h"
#include "common.h"

typedef enum {
    RETURN, CONSTANT, LONG_CONSTANT, UNARY, EXTRACT_BIND, BINARY,
    POP, DECLARE_R_INT_32
} OpCode;

#define SSTR(x) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#define IS_DIGIT(ch) ch >= '0' && ch <= '9'
#define IS_ID(ch) isalnum(ch) || IS_DIGIT(ch)
#define SKIP_WHITESPACES() \
    case ' ': \
        NEXT(); \
    case '\r': \
        NEXT(); \
    case '\t': \
        NEXT(); \
    case '\n': \
        NEXT()

#define NEXT() advance(); break

#define CHAR_PTR(ch) (reinterpret_cast<uint8_t *>(ch))

static map<string, ValueType> typedefs;

// TD is shortened typedef

static ValueType stringToType(string source) {
    const char* cstr = source.c_str();
    if (strcmp(cstr, "r_int32") == 0) return INT;
    else if (strcmp(cstr, "r_chr_ptr") == 0) return STRING;
    return INT;
}

static void typedefsPut(string name, string source) {
    typedefs[name] = stringToType(source);
}

static ValueType typedefsGet(string name) {
    return typedefs[name];
}

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
        case STRING: {
            return value.as.string;
        }
        default:
            return "NULL";
    }
}

static size_t size(Value value) {
    switch (value.type) {
        case SHORT:
            return sizeof(short);
        case INT:
            return sizeof(int);
        case LONG:
            return sizeof(long);
        case DOUBLE:
            return sizeof(double);
        case STRING:
            return sizeof(value.as.string) + sizeof(int);
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
    return "NULL";
}

static char* readFile(const char* path) {
    FILE* file = fopen(path, "rb");

    if (file == NULL) {
        fprintf(stderr, "Could not open file \"%s\".\n", path);
        exit(74);
    }

    fseek(file, 0L, SEEK_END);
    size_t fileSize = ftell(file);
    rewind(file);

    char* buffer = (char*)malloc(fileSize + 1);
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, file);
    if (buffer == NULL) {
        fprintf(stderr, "Not enough memory to read \"%s\".\n", path);
        exit(74);
    }
    if (bytesRead < fileSize) {
        fprintf(stderr, "Could not read file \"%s\".\n", path);
        exit(74);
    }
    buffer[bytesRead] = '\0';

    fclose(file);
    return buffer;
}