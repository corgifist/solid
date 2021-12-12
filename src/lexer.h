#pragma once

#include "common.h"

enum TokenType {
	NUMBER, LPAREN, RPAREN,

	PLUS, MINUS, STAR, SLASH,

	TEOF,
};

typedef struct Token {
	TokenType type;
	int line;
	const char* text;
} Token;


static void lex_error(const char* msg, int line) {
	barley_exception("LexError", msg, line);
}

