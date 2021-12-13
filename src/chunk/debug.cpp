// This script is part of SOLID language.
// Copyright 2021.

#include "debug.h"
#include "../utils.h"


int simpleOffset(int offset, const char* name) {
    print(name);
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

int offsetize(Chunk* chunk, int offset) {
    uint8_t instruction = chunk->code[offset];
    printf("%04d ", offset);
    switch (instruction) {
        case RETURN:
            return simpleOffset(offset, "RETURN");
        case CONST:
            return constOffset(offset, "CONST", chunk);
        default:
            return unknownOffset(offset);
    }
}


void disassemble(Chunk* chunk, const char* name) {
    print("Disassemble of " << name << ":");
    for (int offset = 0; offset < chunk->count;) {
        offset = offsetize(chunk, offset);
    }
}

