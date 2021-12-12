// This script is part of SOLID language.
// Copyright 2021.
#include "common.h"
#include "lexer.h"

#pragma once

#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()