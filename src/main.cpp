#include "lexer.cpp"

Lexer lexer(const char* msg) {
	Lexer lex = Lexer(msg);
	return lex;
}

////////////////////////////////

const char* lexer_to_string(Lexer* lexer) {
    return lexer->getInput();
}

///////////////////////////////

int main() {
	auto lexer = Lexer("hello, world");
    printf("%s",  lexer_to_string(&lexer));
    vector<
	return 0;
}
