// This script is part of SOLID language.
// Copyright 2021.

#include "parser/compiler.h"
#include "utils.h"

void runFile(const char* path) {
    const char* script = readFile(path);
    parse(script);
}

int main() {
    runFile("program.solid");
    return 0;
}
