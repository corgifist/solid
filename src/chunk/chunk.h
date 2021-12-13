// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "../utils.h"
#include "constnants.h"
#include <stdint-gcc.h>
#include <cstdlib>

typedef struct {
    int count, capacity;
    uint8_t* code;
    Constants constants;
} Chunk;

void initChunk(Chunk* chunk);
void writeChunk(Chunk* chunk, uint8_t byte);
void freeChunk(Chunk* chunk);

int writeConstant(Chunk* chunk, Value value);