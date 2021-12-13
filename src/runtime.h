// This script is part of SOLID language.
// Copyright 2021.

#pragma once

#include "common.h"

typedef enum {
    RUNTIME_OK, RUNTIME_ERROR, COMPILE_ERROR
} InterpretResult;

static InterpretResult runtime_result;

static void runtime_check() {
    if (runtime_result == RUNTIME_OK) return;
    else if (runtime_result == RUNTIME_ERROR) exit(1);
}
