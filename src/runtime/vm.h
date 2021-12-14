// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "../common.h"
#include "../chunk/chunk.h"

#define S_SIZE 128

typedef struct {
    uint8_t *stage;
    Chunk *chunk;
    Value stack[S_SIZE];
    Value *stackTop;
} VM;

void initVM(Chunk *chunk);

void freeVM();

InterpretResult interpret();

void resetStack();

void push(Value value);

Value pop();

