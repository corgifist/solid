// This script is part of SOLID language.
// Copyright 2021.

#include "table.h"

std::map<string, Value> table;

namespace Table {
    void put(string key, Value value) {
        table[key] = value;
    }

    Value get(string key) {
        return table[key];
    }

    bool contains(string key) {
        return table.contains(key);
    }

    void clear() {
        table.clear();
    }
}

