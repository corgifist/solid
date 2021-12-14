// This script is part of SOLID language.
// Copyright 2021.

#include "debug.h"

void disassemble(Chunk *chunk, const char *name) {
    print("Disassemble of " << name << ":");
    for (int offset = 0; offset < chunk->count;) {
        offset = offsetize(chunk, offset);
    }
}

int offsetize(Chunk *chunk, int offset) {
    int line = getLine(chunk, offset);
    if (offset != 0) {
        if (getLine(chunk, offset - 1) == line) {
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
        case LONG_CONSTANT:
            return longConstOffset(offset, "LONG_CONSTANT", chunk);
        case EXTRACT_BIND:
            return simpleOffset(offset, "EXTRACT_BIND");
        case UNARY:
            return operatorOffset(offset, "UNARY", chunk);
        case BINARY:
            return operatorOffset(offset, "BINARY", chunk);
        default:
            return unknownOffset(chunk, offset);
    }
}

int simpleOffset(int offset, const char *name) {
    print(name);
    return offset + 1;
}

int unknownOffset(Chunk* chunk,int offset) {
    print("unknown opcode '" << chunk->code[offset] << "'");
    return offset + 1;
}

int constOffset(int offset, const char *name, Chunk *chunk) {
    Value constant = chunk->constants.values[chunk->code[offset + 1]];
    print(name << " " << size(constant) << " %" << type(constant) << " " << " *" << &constant << " '" << object_to_string(constant) << "'");
    return offset + 2;
}

int longConstOffset(int offset, const char*name, Chunk* chunk) {
    Value constant = chunk->constants.values[chunk->code[offset + 1] |
                                             (chunk->code[offset + 2] << 8) |
                                             (chunk->code[offset + 3] << 16)];
    print(name << " " << size(constant) << " %" << type(constant) << " " << " *" << &constant << "'" << object_to_string(constant) << "'");
}

int operatorOffset(int offset, const char *name, Chunk *chunk) {
    print(name << " '" << chunk->code[offset + 1] << "'");
    return offset + 2;
}

