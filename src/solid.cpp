// This script is part of SOLID language.
// Copyright 2021.

#include "lexer.cpp"

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
    Lexer lexer = Lexer("2 + 25.21 * 3.14 + 3.1");
    vector<Token> result = lexer.lex();
    token_vector_to_string(result);
    return 0;
}
