// This script is part of SOLID language.
// Copyright 2021.

#pragma once

typedef enum {
    RETURN
} OpCode;

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

#define IS_DIGIT(ch) ch >= '0' && ch <= '9'

#define SKIP_WHITESPACES() \
    case ' ': \
        break; \
    case '\r': \
        break; \
    case '\t': \
        break; \
    case '\n': \
        break

#define CHAR_PTR(ch) reinterpret_cast<uint8_t *>(ch)