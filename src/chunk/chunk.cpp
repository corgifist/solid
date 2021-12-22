// This script is part of SOLID language.
// Copyright 2021.

#include "chunk.h"
#include "allocations.h"

void initChunk(Chunk *chunk) {
    chunk->count = 0;
    chunk->capacity = 0;
    chunk->code = NULL;
    chunk->lineCount = 0;    // <--
    chunk->lineCapacity = 0; // <--
    chunk->lines = NULL;
    initConstants(&chunk->constants);
}

void writeChunk(Chunk *chunk, uint8_t byte, int line) {
    if (chunk->capacity < chunk->count + 1) {
        int oldCapacity = chunk->capacity;
        chunk->capacity = GROW_CAPACITY(oldCapacity);
        chunk->code = GROW_ARRAY(uint8_t, chunk->code,
                                 oldCapacity, chunk->capacity);
    }

    chunk->code[chunk->count] = byte;
    chunk->count++;

    // See if we're still on the same line.
    if (chunk->lineCount > 0 &&
        chunk->lines[chunk->lineCount - 1].line == line) {
        return;
    }

    // Append a new LineStart.
    if (chunk->lineCapacity < chunk->lineCount + 1) {
        int oldCapacity = chunk->lineCapacity;
        chunk->lineCapacity = GROW_CAPACITY(oldCapacity);
        chunk->lines = GROW_ARRAY(LineStart, chunk->lines,
                                  oldCapacity, chunk->lineCapacity);
    }

    LineStart* lineStart = &chunk->lines[chunk->lineCount++];
    lineStart->offset = chunk->count - 1;
    lineStart->line = line;
}

void freeChunk(Chunk *chunk) {
    FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
    FREE_ARRAY(LineStart, chunk->lines, chunk->lineCapacity);
    freeConstants(&chunk->constants);
    initChunk(chunk);
}

int addConstant(Chunk* chunk, Value value) {
   writeConstants(&chunk->constants, value);
   return chunk->constants.count - 1;
}

int writeConstant(Chunk *chunk, Value value, int line) {
    int index = addConstant(chunk, value);
    if (index < 255) {
        writeChunk(chunk, CONSTANT, line);
        writeChunk(chunk, (uint8_t)index, line);
    } else {
        writeChunk(chunk, LONG_CONSTANT, line);
        writeChunk(chunk, (uint8_t)(index & 0xff), line);
        writeChunk(chunk, (uint8_t)((index >> 8) & 0xff), line);
        writeChunk(chunk, (uint8_t)((index >> 16) & 0xff), line);
    }
    return index;
}

int getLine(Chunk* chunk, int instruction) {
    int start = 0;
    int end = chunk->lineCount - 1;

    for (;;) {
        int mid = (start + end) / 2;
        LineStart* line = &chunk->lines[mid];
        if (instruction < line->offset) {
            end = mid - 1;
        } else if (mid == chunk->lineCount - 1 ||
                   instruction < chunk->lines[mid + 1].offset) {
            return line->line;
        } else {
            start = mid + 1;
        }
    }
}
