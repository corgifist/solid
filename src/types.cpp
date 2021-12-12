// This script is part of SOLID language.
// Copyright 2021.

#include "common.h"
#include "utils.cpp"

enum ValueType {
    NUMBER,
};

class Value {

public:
    virtual double asNumber() const = 0;

    virtual string asString() const = 0;

    virtual ValueType type() const = 0;
};

class NumberValue : Value {

private:
    double number;

public:

    NumberValue(double value) {
        this->number = value;
    }

    virtual double asNumber() {
        return number;
    }

    virtual string asString() {
        return SSTR(number);
    }

    virtual ValueType type() {
        return NUMBER;
    }
};
