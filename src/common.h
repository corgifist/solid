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