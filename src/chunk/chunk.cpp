// This script is part of SOLID language.
// Copyright 2021.

#include "chunk.h"
#include "allocations.h"

void initChunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lines = NULL;
    initConstants(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t code, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int old = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(old);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code, old, chunk->capacity);
        chunk->lines = GROW_ARRAY(int, chunk->lines, old, chunk->capacity);
    }

    chunk->code[chunk->count] = code;
    chunk->lines[chunk->count] = line;
    chunk->count++;
}

void freeChunk(Chunk *chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    freeConstants(&chunk->constants);
    initChunk(chunk);
}

int writeConstant(Chunk *chunk, Value value) {
    writeConstants(&chunk->constants, value);
    return chunk->constants.count - 1;
}


