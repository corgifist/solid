// This script is part of SOLID language.
// Copyright 2021.

#include "parser/compiler.h"
#include "runtime/table.h"

int main() {
    Table::put("name", INT(1));
    parse("typedef int -> r_int32; r_int32 a = 5; int c = 12; a + c;");
    return 0;
}
