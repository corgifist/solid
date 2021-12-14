// This script is part of SOLID language.
// Copyright 2021.

#include "compiler.h"
#include "parser.cpp"

void parse(const char* msg) {
    Lexer lexer = Lexer(msg);
    vector<Token> result = lexer.lex();
    Parser parser = Parser(result);
    Chunk chunk = parser.parse();
    disassemble(&chunk, "compiler test");
    initVM(&chunk);
    interpret();
}
