// This script is part of SOLID language.
// Copyright 2021.

#include "lexer.cpp"
#include "chunk/chunk.h"
#include "chunk/debug.h"
#include "values/type.h"

Lexer lexer(const char* msg) {
    Lexer lex = Lexer(msg);
    return lex;
}

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
    int position = writeConstant(&chunk, INT_VALUE(1));
    writeChunk(&chunk, CONST);
    writeChunk(&chunk, position);
    disassemble(&chunk, "test chunk");
    return 0;
}
