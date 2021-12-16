// This script is part of SOLID language.
// Copyright 2021.

#include "table.h"

#include <utility>

typedef struct Local {
    int depth{};
    string name;
    Variable variable{};

    [[nodiscard]] string toString() const {
        return name + " " + to_string(depth);
    }

    [[nodiscard]] bool isValid() const {
        return name.empty();
    }
} Local;

class Table {
    int depth;
    int count;
    Local locals[512];

public:
    Table() {
        this->depth = 1;
        this->count = 0;
    }

    void put(string name, Value value) {
        Local local = genLocal(false, value, std::move(name));
        this->locals[this->count++] = local;
    }

    Value get(const string& name) {
        for (const Local& local : locals) {
            if (local.name == name) {
                return local.variable.value;
            }
        }
        return INT(0);
    }

    void push() {
        this->depth++;
    }

    void pop() {
        Local newLocals[512];
        this->depth--;
        for (int i = 0; i < 512; i++) {
            if (locals[i].depth < this->depth) continue;
            newLocals[i] = locals[i];
        }
        for (int i = 0; i < 512; i++) {
            this->locals[i] = newLocals[i];
        }
    }

    bool contains(const string& name) {
       for (const Local& local : locals) {
           if (local.name.empty()) break;
           if (local.name == name) {
               return true;
           }
       }

       return false;
    }

    static Local genLocal(bool constant, Value value, string name) {
        Local local;
        local.name = std::move(name);
        local.variable = genVar(value, constant);
        return local;
    }

    static Variable genVar(Value value, bool constant) {
        return Variable({constant, value});
    }
};