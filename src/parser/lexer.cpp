// This script is part of SOLID language.
// Copyright 2021.

#include "lexer.h"

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
        acc += SSTR(line);
        acc += " ";
        acc += text;
        return acc;
    }
};

class Lexer {
private:
    string input;
    unsigned int length, pos, line;
    vector<Token> result;

    Token token(TokenType type, string msg) const {
        return Token(type, line, msg);
    }

    char peek(int relativePos) {
        int finalPos = pos + relativePos;
        if (finalPos >= length) return '\0';
        return input.at(finalPos);
    }

    char advance() {
        char rest = peek(0);
        if (rest == '\n') line++;
        pos++;
        return rest;
    }

    bool end() const {
        return pos >= length;
    }

    void addToken(TokenType type, string msg) {
        result.push_back(token(type, msg));
    }

    void illegal_character(char current) const {
        string acc = "illegal character '";
        acc = acc + current;
        acc = acc + "'";
        lex_error(acc, line);
    }

    void number() {
        string acc;
        while (IS_DIGIT(peek(0)) || peek(0) == '.') {
            if (peek(0) == '.') {
                if (acc.find('.') != string::npos) {
                    lex_error("invalid float number", line);
                }
            }
            acc += peek(0);
            advance();
        }
        addToken(acc.find('.') == string::npos ? "INT" : "FLOAT", acc);
    }

public:
    explicit Lexer(const char *source) {
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
                    NEXT();
                case '-':
                    addToken("MINUS", "-");
                    NEXT();
                case '*':
                    addToken("STAR", "*");
                    NEXT();
                case '/':
                    addToken("SLASH", "/");
                    NEXT();
                case ';':
                    addToken("SEMICOLON", ";");
                    NEXT();

                SKIP_WHITESPACES();

                default: // checks
                    if (IS_DIGIT(current)) {
                        number();
                    } else illegal_character(current);
            }
        }
        addToken("EOF", "\0");
        return result;
    }

    void setInput(const char *msg) {
        input = msg;
    }

    string getInput() const {
        return input;
    }

    int getPos() const {
        return pos;
    }
};
