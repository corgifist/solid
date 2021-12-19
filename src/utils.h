// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "values/type.h"
#include "common.h"

typedef enum {
    RETURN, CONSTANT, LONG_CONSTANT, UNARY, EXTRACT_BIND, BINARY,
    POP, DECLARE_R_INT_32, DECLARE_R_INT_16, DECLARE_R_INT_64,
    CAST, ASSIGN, PRINT, DECLARE_R_FLOAT_64, DECLARE_R_CHR_PTR,
    DECLARE_R_BOOL_1, DECLARE_R_BYTE_8, SCOPE_START, SCOPE_END,
    DECLARE_U_BYTE_8, DECLARE_U_SHRT_16, DECLARE_U_INT_32,
    DECLARE_U_INT_64, JUMP_IF_FALSE, JUMP_ANYWAY, CONSTANTIFY,
    DECLARE_R_CHR_8, LOOP, DUP, BUILD_SWITCH, BEGIN_SWITCH, SWITCH_TABLE
} OpCode;

#define SSTR(x) dynamic_cast< std::ostringstream & >( \
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

static ValueType stringToType(const string& source) {
    const char* cstr = source.c_str();
    if (strcmp(cstr, "r_int32") == 0) return INT;
    else if (strcmp(cstr, "r_shrt16") == 0) return SHORT;
    else if (strcmp(cstr, "r_int64") == 0) return LONG;
    else if (strcmp(cstr, "r_float64") == 0) return DOUBLE;
    else if (strcmp(cstr, "r_chr_ptr") == 0) return STRING;
    else if (strcmp(cstr, "r_bool1") == 0) return BOOL;
    else if (strcmp(cstr, "r_byte8") == 0) return BYTE;
    else if (strcmp(cstr, "u_byte8") == 0) return UNSIGNED_BYTE;
    else if (strcmp(cstr, "u_shrt16") == 0) return UNSIGNED_SHORT;
    else if (strcmp(cstr, "u_int32") == 0) return UNSIGNED_INT;
    else if (strcmp(cstr, "u_int64") == 0) return UNSIGNED_LONG;
    else if (strcmp(cstr, "r_chr8") == 0) return CHAR;
    else if (typedefs.contains(source)) {
        return typedefs[source];
    }
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
        case BYTE:
            return (double) operand.as.byte;
        case DOUBLE:
            return operand.as.dbl;
        case UNSIGNED_BYTE:
            return (double) operand.as.unbyte;
        case UNSIGNED_SHORT:
            return (double) operand.as.unshr;
        case UNSIGNED_INT:
            return (double) operand.as.unnt;
        case UNSIGNED_LONG:
            return (double) operand.as.unlgn;
        case CHAR:
            return (double) operand.as.byte;
    }
    return 0;
}

static Value NUMBER_FROM_SOURCE(Value source, double operand) {
    switch (source.type) {
        case BYTE:
            return BYTE((char) operand);
        case SHORT:
            return SHORT((short) operand);
        case INT:
            return INT((int) operand);
        case LONG:
            return LONG((long) operand);
        case UNSIGNED_BYTE:
            return UNSIGNED_BYTE((unsigned char) operand);
        case UNSIGNED_INT:
            return UNSIGNED_INT((unsigned int) operand);
        case UNSIGNED_SHORT:
            return UNSIGNED_SHORT((unsigned short) operand);
        case UNSIGNED_LONG:
            return UNSIGNED_LONG((unsigned long) operand);
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
            case BYTE:
                return SSTR((int) value.as.byte);
            case UNSIGNED_BYTE:
                return SSTR((unsigned int) value.as.unbyte);
            case UNSIGNED_SHORT:
                return SSTR(value.as.unshr);
            case UNSIGNED_INT:
                return SSTR(value.as.unnt);
            case UNSIGNED_LONG:
                return SSTR(value.as.unlgn);
        }
    }
    switch (value.type) {
        case STRING: {
            return value.as.string;
        }
        case BOOL:
            return value.as.boolean ? "true" : "false";
        case CHAR:
            return SSTR(value.as.byte);
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
        case BOOL:
            return sizeof(bool);
        case BYTE:
            return sizeof(unsigned char);
        case UNSIGNED_BYTE:
            return sizeof(unsigned char);
        case UNSIGNED_SHORT:
            return sizeof(unsigned short);
        case UNSIGNED_INT:
            return sizeof(unsigned int);
        case UNSIGNED_LONG:
            return sizeof(unsigned long);
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
        case BOOL:
            return "bool";
        case BYTE:
            return "byte";
        case UNSIGNED_BYTE:
            return "u_byte";
        case UNSIGNED_SHORT:
            return "u_short";
        case UNSIGNED_INT:
            return "u_int";
        case UNSIGNED_LONG:
            return "u_long";
        case CHAR:
            return "char";
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