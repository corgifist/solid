// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "common.h"

typedef string TokenType;

static void lex_error(string msg, int line) {
	barley_exception("LexError", msg, line);
    runtime_result = COMPILE_ERROR;
}

