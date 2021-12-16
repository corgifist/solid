// This script is part of SOLID language.
// Copyright 2021.

#include "debug.h"

int jumpOffset(int offset, int sign, const char *string, Chunk *chunk);

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
        case DECLARE_R_INT_16:
            return constOffset(offset, "DECLARE_R_INT_16", chunk);
        case DECLARE_R_INT_32:
            return constOffset(offset, "DECLARE_R_INT_32", chunk);
        case DECLARE_R_INT_64:
            return constOffset(offset, "DECLARE_R_INT_64", chunk);
        case DECLARE_R_FLOAT_64:
            return constOffset(offset, "DECLARE_R_FLOAT_64", chunk);
        case DECLARE_R_CHR_PTR:
            return constOffset(offset, "DECLARE_R_CHR_PTR", chunk);
        case DECLARE_R_BOOL_1:
            return constOffset(offset, "DECLARE_R_BOOL_1", chunk);
        case DECLARE_R_BYTE_8:
            return constOffset(offset, "DECLARE_R_BYTE_8", chunk);
        case DECLARE_U_BYTE_8:
            return constOffset(offset, "DECLARE_U_BYTE_8", chunk);
        case DECLARE_U_SHRT_16:
            return constOffset(offset, "DECLARE_U_SHRT_16", chunk);
        case DECLARE_U_INT_32:
            return constOffset(offset, "DECLARE_U_INT_32", chunk);
        case DECLARE_U_INT_64:
            return constOffset(offset, "DECLARE_U_INT_64", chunk);
        case RETURN:
            return simpleOffset(offset, "RETURN");
        case CONSTANT:
            return constOffset(offset, "CONSTANT", chunk);
        case LONG_CONSTANT:
            return longConstOffset(offset, "LONG_CONSTANT", chunk);
        case EXTRACT_BIND:
            return constOffset(offset, "EXTRACT_BIND", chunk);
        case UNARY:
            return operatorOffset(offset, "UNARY", chunk);
        case BINARY:
            return operatorOffset(offset, "BINARY", chunk);
        case POP:
            return simpleOffset(offset, "POP");
        case JUMP_ANYWAY:
            return jumpOffset(offset, 1, "JUMP_ANYWAY", chunk);
        case JUMP_IF_FALSE:
            return jumpOffset(offset, 1, "JUMP_IF_FALSE", chunk);
        case CAST:
            return constOffset(offset, "CAST", chunk);
        case ASSIGN:
            return constOffset(offset, "ASSIGN", chunk);
        case PRINT:
            return simpleOffset(offset, "PRINT");
        case SCOPE_START:
            return simpleOffset(offset, "SCOPE_START");
        case SCOPE_END:
            return simpleOffset(offset, "SCOPE_END");
        default:
            return unknownOffset(chunk, offset);
    }
}

int jumpOffset(int offset, int sign, const char *string, Chunk *chunk) {
    uint16_t jump = (uint16_t)(chunk->code[offset + 1] << 8);
    jump |= chunk->code[offset + 2];
    print(string << " " << offset << " >> " << offset + 3 + sign * jump);
    return offset + 3;
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
    print(name << " " << size(constant) << " %" << type(constant) << " " << " *" << &offset << " '" << object_to_string(constant) << "'");
    return offset + 2;
}

int longConstOffset(int offset, const char*name, Chunk* chunk) {
    uint32_t index = chunk->code[offset + 1] |
                                             (chunk->code[offset + 2] << 8) |
                                             (chunk->code[offset + 3] << 16);
    Value constant = chunk->constants.values[index];
    print(name << " " << size(constant) << " %" << type(constant) << " " << " *" << &constant << " '" << object_to_string(constant) << "'");
    return offset + 4;
}

int operatorOffset(int offset, const char *name, Chunk *chunk) {
    print(name << " '" << chunk->code[offset + 1] << "'");
    return offset + 2;
}

