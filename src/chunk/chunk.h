// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "constants.h"
#include <cstdlib>
#include <string>
#include <vector>

typedef struct {
    int offset;
    int line;
} LineStart;

typedef struct {
    int count, capacity;
    u_int8_t *code;
    Constants constants;
    int lineCount;
    int lineCapacity;
    LineStart* lines;
} Chunk;

typedef struct {
    int arity;
    std::vector<ValueType> types;
    std::vector<std::string> names;
    Chunk chunk;
} Clause;

typedef struct {
    std::vector<Clause> clauses;
    std::string name;
} ObjFunction;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, uint8_t byte, int line);

void freeChunk(Chunk *chunk);

int addConstant(Chunk* chunk, Value value);

int writeConstant(Chunk *chunk, Value value, int line);

int getLine(Chunk* chunk, int instruction);