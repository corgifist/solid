// This script is part of SOLID language.
// Copyright 2021.

#include "lexer.h"
#include "../utils.h"
#include<stack>

#define LEXER_RUNTIME_ERROR() runtime_result = RUNTIME_ERROR

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
    std::map<string, ValueType> typedefs;

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

    bool match(char ch) {
        char next = peek(1);
        if (next == ch) {
            advance();
            return true;
        } else return false;
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

    void id() {
        string acc;
        while (IS_ID(peek(0)) || peek(0) == '!' || peek(0) == '?' || peek(0) == '_') {
            acc += peek(0);
            advance();
        }

        if (acc == "r_int32") addToken("R_INT32", "r_int32");
        else if (acc == "typedef") addToken("TYPEDEF", "typedef");
        else if (acc == "r_shrt16") addToken("R_SHRT16", "r_shrt16");
        else if (acc == "r_int64") addToken("R_INT64", "r_int64");
        else if (acc == "r_float64") addToken("R_FLOAT64", "r_float64");
        else if (acc == "r_chr_ptr") addToken("R_CHR_PTR", "r_chr_ptr");
        else if (acc == "print") addToken("PRINT", "print");
        else if (acc == "true") addToken("TRUE", "true");
        else if (acc == "false") addToken("FALSE", "false");
        else if (acc == "if") addToken("IF", "if");
        else if (acc == "else") addToken("ELSE", "else");
        else if (acc == "r_bool1") addToken("R_BOOL1", "r_bool1");
        else if (acc == "r_byte8") addToken("R_BYTE8", "r_byte8");
        else if (acc == "u_byte8") addToken("U_BYTE8", "u_byte8");
        else if (acc == "u_shrt16") addToken("U_SHRT16", "u_shrt16");
        else if (acc == "u_int32") addToken("U_INT32", "u_int32");
        else if (acc == "u_int64") addToken("U_INT64", "u_int64");
        else if (acc == "and") addToken("AND", "and");
        else if (acc == "const") addToken("CONST", "const");
        else if (acc == "r_chr8") addToken("R_CHR8", "r_chr8");
        else if (acc == "or") addToken("OR", "or");
        else if (acc == "while") addToken("WHILE", "while");
        else addToken("ID", acc);
    }

    void lex_string() {
        string acc;

        advance();
        while (peek(0) != '"') {
            char current = peek(0);
            if (current == '\0') {
                lex_error("reached eof while parsing string", line);
                runtime_check();
            }
            if (current == '\\') {
                current = advance();
                switch (current) {
                    case '\\': acc += "\\"; break;
                    case 'n': acc += "\n"; break;
                    case 't': acc += "\t"; break;
                    case 'r': acc += "\r"; break;
                    case 'b': acc += "\b"; break;
                    default: acc += "\\"; break;
                }
            }
            else acc += current;
            advance();
        }
        advance();

        addToken("STRING", acc);
    }

    void sync() {
        while (!end()) {
            advance();
        }
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
                    if (match('>')) addToken("STABBER", "->");
                    else addToken("MINUS", "-");
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
                case '(':
                    addToken("LPAREN", "(");
                    NEXT();
                case ')':
                    addToken("RPAREN", ")");
                    NEXT();
                case '=':
                    addToken(match('=') ? "EQEQ" : "EQ", "=");
                    NEXT();
                case '"':
                    lex_string();
                    break;
                case '{':
                    addToken("LBRACE", "{");
                    NEXT();
                case '}':
                    addToken("RBRACE", "}");
                    NEXT();
                case '<':
                    addToken(match('=') ? "LTEQ" : "LT", "<");
                    NEXT();
                case '>':
                    addToken(match('=') ? "GTEQ" : "GT", ">");
                    NEXT();
                case '!':
                    addToken(match('=') ? "EXCLEQ" : "EXCL", "!");
                    NEXT();

                SKIP_WHITESPACES();

                default: // checks
                    if (IS_DIGIT(current)) {
                        number();
                    } else if (IS_ID(current)) {
                        id();
                    } else {
                        illegal_character(current);
                        LEXER_RUNTIME_ERROR();
                        sync();
                        break;
                    }
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
