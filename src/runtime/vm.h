// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include <stack>
#include <unordered_map>
#include "../common.h"
#include "../chunk/chunk.h"
#include "table.cpp"

#define FRAMES_MAX 128

typedef struct {
    int stage;
    Chunk* function;
    stack<Value> st;
} CallFrame;

typedef struct {
    stack<CallFrame> frames;
    Table table;
    bool constant;
    std::stack<std::unordered_map<string, int>> switchStack;
} VM;

void initVM(Chunk *chunk);

void freeVM();

void traceVM();

void frameVM(CallFrame frame);

void newSwitch();

void pushSwitch(Value value, int jump);

unordered_map<string, int> popSwitch();

InterpretResult interpret();

void concat(Value a, Value b);

void resetStack();

void push(Value value);

Value pop();

Value peek(int relative);

bool isFalse(Value value);
