// This script is part of SOLID language.
// Copyright 2021.

#include "debug.h"
#include "../utils.h"

void disassemble(Chunk* chunk, const char* name) {
    print("Disassemble of " << name << ":");
    for (int offset = 0; offset < chunk->count;) {
        offset = offsetize(chunk, offset);
    }
}

int offsetize(Chunk* chunk, int offset) {
    int line = chunk->lines[offset];
    if (offset != 0) {
        if (chunk->lines[offset - 1] == line) {
            printf("| ");
        } else {
            printf("%d ", line);
        }
    } else {
        printf("%d ", line);
    }

    uint8_t instruction = chunk->code[offset];
    printf("%04d ", offset);
    switch (instruction) {
        case RETURN:
            return simpleOffset(offset, "RETURN");
        case CONSTANT:
            return constOffset(offset, "CONSTANT", chunk);
        case EXTRACT_BIND:
            return constOffset(offset, "EXTRACT_BIND", chunk);
        case UNARY:
            return unaryOffset(offset, "UNARY", chunk);
        default:
            return unknownOffset(offset);
    }
}


int simpleOffset(int offset, const char* name) {
    print(name);
    if (strcmp(name, "RETURN") == 0) {
        print("!");
    }
    return offset + 1;
}

int unknownOffset(int offset) {
    print("unknown opcode");
    return offset + 1;
}

int constOffset(int offset, const char* name, Chunk* chunk) {
    Value constant = chunk->constants.values[chunk->code[offset + 1]];
    print(name << " " << size(constant) << " *" << &constant << " '" << object_to_string(constant) << "'");
    return offset + 2;
}

int unaryOffset(int offset, const char* name, Chunk* chunk) {
    print(name << " '" << chunk->code[offset + 1] << "'");
    return offset + 2;
}

