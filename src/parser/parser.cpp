// This script is part of SOLID language.
// Copyright 2021.

#include "parser.h"

#include <utility>
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
            parse_exception(std::move(msg), line());
            return current;
        }
        pos++;
        return current;
    }

    void identifierConstant(string text) {
        char* buffer = static_cast<char *>(malloc(sizeof(char) * text.length()));
        for (int i = 0; i < text.length(); i++) {
            buffer[i] = text.at(i);
        }
        buffer[text.length()] = '\0';
        emitByte(addConstant(&chunk, STRING(buffer)));
    }

    void advance_parser() {
        match(get(0).getType());
    }

    string valueTypeFromString(ValueType type) {
        switch (type) {
            case SHORT:
                return "r_shrt16";
            case INT:
                return "r_int32";
            case LONG:
                return "r_int64";
            case DOUBLE:
                return "r_float64";
            case STRING:
                return "r_chr_ptr";
            case BOOL:
                return "r_bool1";
            case BYTE:
                return "r_byte8";
            case UNSIGNED_BYTE:
                return "u_byte8";
            case UNSIGNED_SHORT:
                return "u_shrt16";
            case UNSIGNED_INT:
                return "u_int32";
            case UNSIGNED_LONG:
                return "u_int64";
            case CHAR:
                return "r_chr8";
            default:
                return "r_int32";
        }
    }

    bool lookMatch(int rel, TokenType type) {
        return get(rel).getType() == type;
    }

    int emitJump(uint8_t instruction) {
        emitByte(instruction);
        emitBytes(0xff, 0xff);
        return chunk.count - 2;
    }

    void patchJump(int offset) {
        int jump = chunk.count - offset - 2;

        if (jump > 65333) {
            barley_exception("JumpFailure", "too much code to jump over", line());
            PARSER_RUNTIME_ERROR();
            runtime_check();
        }

        chunk.code[offset] = (jump >> 8) & 0xff;
        chunk.code[offset + 1] = jump & 0xff;
    }

public:
    explicit Parser(const vector<Token>& tokens) {
        this->tokens = tokens;
        this->pos = 0;
        this->size = tokens.size();
        initChunk(&chunk);

        typedefsPut("short", "r_shrt16");
        typedefsPut("int", "r_int32");
        typedefsPut("long", "r_int64");
        typedefsPut("float", "r_float64");
        typedefsPut("string", "r_chr_ptr");
        typedefsPut("bool", "r_bool1");
        typedefsPut("byte", "r_byte8");
        typedefsPut("char", "r_chr8");
        typedefsPut("unsigned_byte", "u_byte8");
        typedefsPut("unsigned_short", "u_shrt16");
        typedefsPut("unsigned_int", "u_int32");
        typedefsPut("unsigned_long", "u_int64");
    }

    void statementOrBlock() {
        if (lookMatch(0, "LBRACE")) block();
        else declaration();
    }

    void block() {
        consume("LBRACE", "expected '{' in block expression");
        emitByte(SCOPE_START);
        while (!match("RBRACE")) {
            declaration();
            consume("SEMICOLON", "unterminated expression in block");
            runtime_check();
        }

        emitByte(SCOPE_END);
    }

    Chunk parse() {
        while(!match("EOF")) {
            declaration();
            consume("SEMICOLON", "expected ';' after expression");
        }
        writeChunk(&chunk, RETURN, line());
        return chunk;
    }

    void declaration() {
        string text = get(0).getText();
        if (match("R_INT32")) {
            declare_int_32();
        } else if (match("R_SHRT16")) {
            declare_int_16();
        } else if (match("TYPEDEF")) {
            declare_typedef();
        } else if (typedefs.contains(text)) {
            advance_parser();
            declare_by_type(typedefsGet(text));
        } else if (match("PRINT")) {
            printStatement();
        } else if (match("R_INT64")) {
            declare_int_64();
        } else if (match("R_FLOAT64")) {
            declare_float_64();
        } else if (match("R_CHR_PTR")) {
            declare_chr_ptr();
        } else if (lookMatch(0, "LBRACE")) {
            block();
        } else if (match("R_BOOL1")) {
            declare_r_bool1();
        } else if (match("R_BYTE8")) {
            declare_r_byte_8();
        } else if (match("U_BYTE8")) {
            declare_u_byte_8();
        } else if (match("U_SHRT16")) {
            declare_u_shrt_16();
        } else if (match("U_INT32")) {
            declare_u_int_32();
        } else if (match("U_INT64")) {
            declare_u_int_64();
        } else if (match("IF")) {
            declare_if();
        } else if (match("CONST")) {
            emitByte(CONSTANTIFY);
            declaration();
        } else if (match("R_CHR8")) {
            declare_r_chr_8();
        } else {
            assignment();
        }
    }

    void declare_if() {
        consume("LPAREN", "expected '(' after 'if'");
        expression();
        consume("RPAREN", "expected ')' after 'if'");

        int thenJump = emitJump(JUMP_IF_FALSE);

        statementOrBlock();

        int elseJump = emitJump(JUMP_ANYWAY);

        patchJump(thenJump);

        if (match("ELSE")) statementOrBlock();

        patchJump(elseJump);
    }

    void declare_r_chr_8() {
        string name = consume("ID", "expected identifier at r_chr8 declaration").getText();
        consume("EQ", "expected '=' after identifier at r_chr8 declaration");
        expression();
        emitByte(DECLARE_R_CHR_8);
        identifierConstant(name);
    }

    void declare_u_int_64() {
        string name = consume("ID", "expected identifier at u_int64 declaration").getText();
        consume("EQ", "expected '=' after identifier at u_int64 declaration");
        expression();
        emitByte(DECLARE_U_INT_64);
        identifierConstant(name);
    }

    void declare_u_int_32() {
        string name = consume("ID", "expected identifier at u_int32 declaration").getText();
        consume("EQ", "expected '=' after identifier at u_int32 declaration");
        expression();
        emitByte(DECLARE_U_INT_32);
        identifierConstant(name);
    }

    void declare_u_shrt_16() {
        string name = consume("ID", "expected identifier at u_shrt16 declaration").getText();
        consume("EQ", "expected '=' after identifier at u_shrt16 declaration");
        expression();
        emitByte(DECLARE_U_SHRT_16);
        identifierConstant(name);
    }

    void declare_u_byte_8() {
        string name = consume("ID", "expected identifier at u_byte8 declaration").getText();
        consume("EQ", "expected '=' after identifier at u_byte8 declaration");
        expression();
        emitByte(DECLARE_U_BYTE_8);
        identifierConstant(name);
    }

    void declare_r_bool1() {
        string name = consume("ID", "expected identifier at r_bool1 declaration").getText();
        consume("EQ", "expected '=' after identifier at r_bool1 declaration");
        expression();
        emitByte(DECLARE_R_BOOL_1);
        identifierConstant(name);
    }

    void declare_r_byte_8() {
        string name = consume("ID", "expected identifier at r_byte8 declaration").getText();
        consume("EQ", "expected '=' after identifier at r_byte8 declaration");
        expression();
        emitByte(DECLARE_R_BYTE_8);
        identifierConstant(name);
    }

    void declare_chr_ptr() {
        string name = consume("ID", "expected identifier at r_chr_ptr declaration").getText();
        consume("EQ", "expected '=' after identifier at r_chr_ptr declaration");
        expression();
        emitByte(DECLARE_R_CHR_PTR);
        identifierConstant(name);
    }

    void declare_float_64() {
        string name = consume("ID", "expected identifier at r_float64 declaration").getText();
        consume("EQ", "expected '=' after identifier at r_float64 declaration");
        expression();
        emitByte(DECLARE_R_FLOAT_64);
        identifierConstant(name);
    }

    void declare_int_64() {
        string name = consume("ID", "expected identifier at r_int64 declaration").getText();
        consume("EQ", "expected '=' after identifier at r_int64 declaration");
        expression();
        emitByte(DECLARE_R_INT_64);
        identifierConstant(name);
    }

    void printStatement() {
        expression();
        emitByte(PRINT);
    }

    void assignment() {
        if (lookMatch(0, "ID") && lookMatch(1, "EQ")) {
            string name = consume("ID", "").getText();
            consume("EQ", "expected '=' after id in assignment");
            expression();
            emitByte(ASSIGN);
            identifierConstant(name);
        } else {
            expressionStatement();
        }
    }

    void declare_by_type(ValueType type) {
        switch (type) {
            case SHORT:
                declare_int_16();
                break;
            case INT:
                declare_int_32();
                break;
            case LONG:
                declare_int_64();
                break;
            case DOUBLE:
                declare_float_64();
                break;
            case STRING:
                declare_chr_ptr();
                break;
            case BOOL:
                declare_r_bool1();
                break;
            case BYTE:
                declare_r_byte_8();
                break;
            case UNSIGNED_BYTE:
                declare_u_byte_8();
                break;
            case UNSIGNED_SHORT:
                declare_u_shrt_16();
                break;
            case UNSIGNED_INT:
                declare_u_int_32();
                break;
            case UNSIGNED_LONG:
                declare_u_int_64();
                break;
            case CHAR:
                declare_r_chr_8();
                break;
        }
    }

    void declare_int_16() {
        string name = consume("ID", "expected identifier at r_shrt16 declaration").getText();
        consume("EQ", "expected '=' after identifier at r_shrt16 declaration");
        expression();
        emitByte(DECLARE_R_INT_16);
        identifierConstant(name);
    }

    void declare_typedef() {
        string name = consume("ID", "expected typename after typedef keyword").getText();
        consume("STABBER", "expected '->' after typedef's new type");
        string source = consume(get(0).getType(), "expected typename after '->' in typedef declaration").getText();
        typedefsPut(name, source);
    }


    void declare_int_32() {
        string name = consume("ID", "expected identifier at r_int32 declaration").getText();
        consume("EQ", "expected '=' after identifier at r_int32 declaration");
        expression();
        emitByte(DECLARE_R_INT_32);
        identifierConstant(name);
    }

    void expressionStatement() {
        expression();
        emitByte(POP);
    }

    void expression() {
        or_();
    }

    void or_() {
        and_();

        while (true) {
            if (match("OR")) {
                int elseJump = emitJump(JUMP_IF_FALSE);
                int endJump = emitJump(JUMP_ANYWAY);

                patchJump(elseJump);
                expression();
                patchJump(endJump);
                continue;
            }
            break;
        }
    }

    void and_() {
        condition();

        while (true) {
            if (match("AND")) {
                int endJump = emitJump(JUMP_IF_FALSE);
                expression();
                patchJump(endJump);
                continue;
            }
            break;
        }
    }

    void condition() {
        additive();

        if (match("EQEQ")) {
            additive();
            emitBinary('=');
            return;
        }

        if (match("GTEQ")) {
            additive();
            emitBinary('g');
            return;
        }

        if (match("LTEQ")) {
            additive();
            emitBinary('l');
            return;
        }

        if (match("EXCLEQ")) {
            additive();
            emitByte('!');
            return;
        }
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
            emitConstant(INT(static_cast<int>(stol(current.getText()))));
        } else if (match("FLOAT")) {
            emitConstant(DOUBLE(stod(current.getText().c_str())));
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
        } else if (match("LPAREN")) {
            string maybeText = get(0).getText();
            if (match("R_INT32") || match("R_SHRT16") || match("R_INT64") || match("R_FLOAT64") || match("R_BYTE8") ||
            match("U_BYTE8") || match("U_SHRT16") || match("U_INT32") || match("U_INT64") || typedefs.contains(get(0).getText()) == 1) {
                if (typedefs.contains(get(0).getText())) advance_parser();
                consume("RPAREN", "expected ')' after cast");
                expression();
                emitByte(CAST);
                if (typedefs.contains(maybeText)) {
                    string type = valueTypeFromString(typedefs[maybeText]);
                    identifierConstant(type);
                } else {
                    identifierConstant(maybeText);
                }
                return;
            }
            expression();
            consume("RPAREN", "expected ')' after expression");
            return;
        } else if (match("STRING")) {
            string text = current.getText();
            char* buffer = static_cast<char *>(malloc(sizeof(char) * text.length()));
            for (int i = 0; i < text.length(); i++) {
                buffer[i] = text.at(i);
            }
            buffer[text.length()] = '\0';
            emitConstant(STRING(buffer));
            return;
        } else if (match("TRUE")) {
            emitConstant(BOOL(true));
        } else if (match("FALSE")) {
            emitConstant(BOOL(false));
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

