// This script is part of SOLID language.
// Copyright 2021.

#include "parser.h"
#include "lexer.cpp"
#include "../chunk/chunk.h"
#include "../runtime/table.h"

#define PARSER_RUNTIME_ERROR() runtime_result = RUNTIME_ERROR


class Parser {
private:
    vector<Token> tokens;
    unsigned int pos, size;
    Chunk chunk;
    Token EOF_TOKEN = Token("EOF", 1, "\0");

    Token get(int rel) const {
        int final = pos + rel;
        if (final >= size) return EOF_TOKEN;
        return tokens.at(final);
    }

    bool match(const TokenType& type) {
        Token current = get(0);
        if (current.getType() != type) return false;
        pos++;
        return true;
    }

    void emitBinary(char op) {
        emitBytes(BINARY, op);
    }

    void emitUnary(char op) {
        emitByte(UNARY);
        emitByte(op);
    }

    int line() const {
        return get(0).getLine();
    }

    void parse_exception(const string msg, int line) {
        barley_exception("ParseException", msg, line);
        PARSER_RUNTIME_ERROR();
        sync();
    }

    void sync() {
        while (!match("EOF")) {
            match(get(0).getType());
        }
    }

    Token consume(const TokenType& type, string msg) {
        Token current = get(0);
        if (current.getType() != type) {
            parse_exception(msg, line());
            return current;
        }
        pos++;
        return current;
    }

public:
    explicit Parser(const vector<Token>& tokens) {
        this->tokens = tokens;
        this->pos = 0;
        this->size = tokens.size();
        initChunk(&chunk);
    }

    Chunk parse() {
        while(!match("EOF")) {
            expression();
            consume("SEMICOLON", "expected ';' after expression");
        }
        writeChunk(&chunk, RETURN, line());
        return chunk;
    }

    void expression() {
        additive();
    }

    void additive() {
        multiplicative();

        while (true) {
            if (match("PLUS")) {
                multiplicative();
                emitBinary('+');
                continue;
            }
            if (match("MINUS")) {
                multiplicative();
                emitBinary('-');
                continue;
            }
            break;
        }
    }

    void multiplicative() {
        unary();

        while (true) {
            if (match("STAR")) {
                unary();
                emitBinary('*');
                continue;
            }
            if (match("SLASH")) {
                unary();
                emitBinary('/');
                continue;
            }
            break;
        }
    }

    void unary() {
        if (match("MINUS")) {
            primary();
            emitUnary('-');
            return;
        }

        primary();
    }

    void primary() {
        Token current = get(0);
        if (match("INT")) {
            emitConstant(INT(stoi(current.getText())));
        } else if (match("FLOAT")) {
            emitConstant(DOUBLE(stod(current.getType())));
        } else if (match("ID")) {
            // Copy token's text
            string text = current.getText();
            char* buffer = static_cast<char *>(malloc(sizeof(char) * text.length()));
            for (int i = 0; i < text.length(); i++) {
                buffer[i] = text.at(i);
            }
            buffer[text.length()] = '\0';
            emitByte(EXTRACT_BIND);
            emitByte(addConstant(&chunk, STRING(buffer)));
            return;
        } else {
            parse_exception("unknown expression", line());
            return;
        }

    }
    void emitByte(uint8_t code) {
        writeChunk(&chunk, code, line());
    }

    void emitBytes(uint8_t code, uint8_t code2) {
        emitByte(code);
        emitByte(code2);
    }

    void emitConstant(Value value) {
        writeConstant(&chunk, value, line());
    }
};

