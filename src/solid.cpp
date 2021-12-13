// This script is part of SOLID language.
// Copyright 2021.

#include "lexer.cpp"
#include "chunk/chunk.h"
#include "chunk/debug.h"
#include "values/type.h"
#include "runtime/vm.h"
#include "utils.h"

////////////////////////////////


void dump_lexer(Lexer lex) {
    print("input: " << lex.getInput());
    print("pos: " << lex.getPos());
}

string token_vector_to_string(vector<Token> toks) {
    for (int i = 0; i < toks.size(); i++) {
        print(toks.at(i).asString());
    }
    return "";
}
///////////////////////////////

int main() {
    Chunk chunk;
    initChunk(&chunk);
    int position = writeConstant(&chunk, NUMBER(1));
    int position2 = writeConstant(&chunk, NUMBER(2));
    writeChunk(&chunk, CONSTANT, 1);
    writeChunk(&chunk, position, 1);
    writeChunk(&chunk, UNARY, 1);
    writeChunk(&chunk, '-', 1);
    writeChunk(&chunk, RETURN, 1);
    VM vm;
    disassemble(&chunk, "test chunk");
    initVM(&chunk);
    interpret();
    return 0;
}
