// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include<iostream>
#include<cstdlib>
#include<cstdio>
#include<vector>
#include<cstring>
#include<sstream>
#include<stdint-gcc.h>
#include <cmath>
#include<map>

#include "runtime.h"

using namespace std;

#define print(S) cout << S << endl

typedef enum {
    RETURN, CONSTANT, LONG_CONSTANT, UNARY, EXTRACT_BIND, BINARY,
    POP, DECLARE_R_INT_32, DECLARE_R_INT_16, DECLARE_R_INT_64,
    CAST, ASSIGN, PRINT, DECLARE_R_FLOAT_64, DECLARE_R_CHR_PTR,
    DECLARE_R_BOOL_1, DECLARE_R_BYTE_8, SCOPE_START, SCOPE_END,
    DECLARE_U_BYTE_8, DECLARE_U_SHRT_16, DECLARE_U_INT_32,
    DECLARE_U_INT_64, JUMP_IF_FALSE, JUMP_ANYWAY, CONSTANTIFY,
    DECLARE_R_CHR_8, LOOP, DUP, BUILD_SWITCH, BEGIN_SWITCH, SWITCH_TABLE,
    DECLARE_AUTO, DECLARE_FUNCTION
} OpCode;

class SolidException {
    std::string type, text;
    unsigned int line;

public:
    SolidException(const std::string &type, const std::string &text, unsigned int line) {
        this->type = type;
        this->text = text;
        this->line = line;
    }

    const string &getType() const {
        return type;
    }

    void setType(const string &type) {
        SolidException::type = type;
    }

    const string &getText() const {
        return text;
    }

    void setText(const string &text) {
        SolidException::text = text;
    }

    unsigned int getLine() const {
        return line;
    }

    void setLine(unsigned int line) {
        SolidException::line = line;
    }
};

static void barley_exception(string type, string msg, int line) {
    runtime_result = RUNTIME_ERROR;
    throw SolidException(type, msg, line);
}

static void system_exception(string type, string msg) {
    runtime_result = RUNTIME_ERROR;
    print("** (" << type << ") " << msg);
}
