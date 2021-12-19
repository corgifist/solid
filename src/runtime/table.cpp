// This script is part of SOLID language.
// Copyright 2021.

#include "table.h"
#include "../utils.h"

#include <utility>

typedef struct Local {
    int depth{};
    string name;
    Variable variable{};

    string toString() const {
        return name + " & depth: " + to_string(depth) + " | " + string("const: ") + to_string(variable.isConstant) + " & value: " + object_to_string(variable.value);
    }
} Local;

class Table {
    int depth;
    int count;
    Local locals[512];

public:
    Table() {
        this->depth = 0;
        this->count = 0;
    }

    void put(string name, Value value) {
        Local newLocals[512];
        for (int i = 0; i < 512; i++) {
            if (locals[i].name == name) continue;
            newLocals[i] = locals[i];
        }
        for (int i = 0; i < 512; i++) {
            this->locals[i] = newLocals[i];
        }
        Local local = genLocal(false, value, std::move(name));
        this->locals[this->count++] = local;
    }

    void assign(string name, Value value) {
        Local newLocals[512];
        int sourceDepth = 0;
        for (int i = 0; i < 512; i++) {
            if (locals[i].name == name) {
                sourceDepth = locals[i].depth;
                continue;
            }
            newLocals[i] = locals[i];
        }
        for (int i = 0; i < 512; i++) {
            this->locals[i] = newLocals[i];
        }
        Local local = genLocal(false, value, std::move(name));
        local.depth = sourceDepth;
        this->locals[this->count++] = local;
    }

    void constant(string name, Value value) {
        Local newLocals[512];
        for (int i = 0; i < 512; i++) {
            if (locals[i].name == name) continue;
            newLocals[i] = locals[i];
        }
        for (int i = 0; i < 512; i++) {
            this->locals[i] = newLocals[i];
        }
        Local local = genLocal(true, value, std::move(name));
        this->locals[this->count] = local;
        this->count++;
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
            if (locals[i].name.empty()) continue;
//            print(locals[i].toString());
//            print("depth: " << this->depth);
            if (locals[i].depth > this->depth) {
                this->count--;
                continue;
            }
            newLocals[i] = locals[i];
        }
        for (int i = 0; i < 512; i++) {
            this->locals[i] = newLocals[i];
        }
    }

    bool contains(const string& name) {
       for (const Local& local : locals) {
           if (local.name == name) {
               return true;
           }
       }

       return false;
    }

    bool isConstant(const string& name) {
        for (const Local& local : locals) {
            if (local.name == name && local.variable.isConstant) return true;
        }
        return false;
    }

    Local genLocal(bool constant, Value value, string name) {
        Local local;
        local.name = std::move(name);
        local.variable = genVar(value, constant);
        local.depth = this->depth;
        return local;
    }

    static Variable genVar(Value value, bool constant) {
        return Variable({constant, value});
    }
};