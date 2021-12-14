// This script is part of SOLID language.
// Copyright 2021.

#include "parser/compiler.h"
#include "runtime/table.h"

int main() {
    Table::put("name", INT(1));
    parse("name;");
    return 0;
}
