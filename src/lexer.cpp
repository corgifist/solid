// This script is part of SOLID language.
// Copyright 2021.

#include "lexer.h"
#include "common.h"

class Token {
    TokenType type;
    int line;
    string text;

public:
    Token(TokenType type, unsigned int line, string text) {
        this->type = type;
        this->line = line;
        this->text = text;
    }

    TokenType getType() const {
        return type;
    }

    int getLine() const {
        return line;
    }

    string getText() const {
        return text;
    }

    string asString() {
        string acc = "";
        acc += type;
        acc += " ";
        acc += line;
        acc += " ";
        acc += text;
        return acc;
    }
};

class Lexer {
private:
    const char* input;
    unsigned int length, pos, line;
    vector<Token> result;

    Token token(TokenType type, string msg) {
        return Token(type, line, msg);
    }

    char peek(int relativePos) {
        string str = input;
        int finalPos = pos + relativePos;
        char at = str.at(finalPos);
        if (at == '\0') return '\0';
        return at;
    }

    char advance() {
        char result = peek(0);
        if (result == '\n') line++;
        pos++;
        return result;
    }

    bool end() {
        return pos >= length;
    }

    void addToken(TokenType type, string msg) {
        result.push_back(token(type, msg));
    }

    void illegal_character(char current) {
        string acc = "illegal character '";
        acc = acc + current;
        acc = acc + "'";
        lex_error(acc, line);
    }

public:
    Lexer(const char* source) {
        input = source;
        length = strlen(source);
        line = 1;
        pos = 0;
    }

    vector<Token> lex() {
        while (!end()) {
            char current = peek(0);
            switch (current) {
                case '+':
                    addToken("PLUS", "+");
                    break;
                case '-':
                    addToken("MINUS", "-");
                    break;
                case '*':
                    addToken("STAR", "*");
                    break;
                case '/':
                    addToken("SLASH", "/");
                    break;
                case ' ':
                    break;
                default: illegal_character(current);
            }
            advance();
        }
        return result;
    }

    void setInput(const char* msg) {
        input = msg;
    }

    const char* getInput() const {
        return input;
    }

    int getPos() const {
        return pos;
    }
};
