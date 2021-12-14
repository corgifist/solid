// This script is part of SOLID language.
// Copyright 2021.

#include "compiler.h"
#include "parser.cpp"

void dump_tokens(vector<Token> tokens) {
    for (auto & token : tokens) {
        print(token.asString());
    }
}

void parse(const char* msg) {
    Lexer lexer = Lexer(msg);
    vector<Token> result = lexer.lex();
    runtime_check();
    dump_tokens(result);
    Parser parser = Parser(result);
    Chunk chunk = parser.parse();
    runtime_check();
    disassemble(&chunk, "compiler test");
    initVM(&chunk);
    interpret();
}
