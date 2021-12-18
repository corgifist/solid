// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "../utils.h"
#include "constants.h"
#include <cstdlib>

typedef struct {
    int offset;
    int line;
} LineStart;

typedef struct {
    int count, capacity;
    uint8_t *code;
    Constants constants;
    int lineCount;
    int lineCapacity;
    LineStart* lines;
} Chunk;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, uint8_t byte, int line);

void freeChunk(Chunk *chunk);

int addConstant(Chunk* chunk, Value value);

int writeConstant(Chunk *chunk, Value value, int line);

int getLine(Chunk* chunk, int instruction);