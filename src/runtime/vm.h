// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "../common.h"
#include "../chunk/chunk.h"

#define S_SIZE 2048

typedef struct {
    uint8_t *stage;
    Chunk *chunk;
    Value* stack;
    int stackCount;
    int stackCapacity;
} VM;

void initVM(Chunk *chunk);

void freeVM();

InterpretResult interpret();

void resetStack();

void push(Value value);

Value pop();

