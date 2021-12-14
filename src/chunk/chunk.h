// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "../utils.h"
#include "constnants.h"
#include <cstdlib>

typedef struct {
    int count, capacity;
    uint8_t *code;
    Constants constants;
    int *lines;
} Chunk;

void initChunk(Chunk *chunk);

void writeChunk(Chunk *chunk, uint8_t byte, int line);

void freeChunk(Chunk *chunk);

int writeConstant(Chunk *chunk, Value value);