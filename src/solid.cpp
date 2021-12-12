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

///////////////////////////////

int main() {
    Lexer lexer = Lexer("hello, world");
    dump_lexer(lexer);
    return 0;
}
