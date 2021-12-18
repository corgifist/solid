// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "../common.h"
#include "../chunk/chunk.h"
#include "table.cpp"

typedef struct {
    uint8_t *stage;
    Chunk *chunk;
    Value* stack;
    int stackCount;
    int stackCapacity;
    Table table;
    bool constant;
} VM;

void initVM(Chunk *chunk);

void freeVM();

InterpretResult interpret();

void concat(Value a, Value b);

void resetStack();

void push(Value value);

Value pop();

Value peek(int relative);

bool isFalse(Value value);
