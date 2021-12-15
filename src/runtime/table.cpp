// This script is part of SOLID language.
// Copyright 2021.

#include "table.h"

#include <utility>

class Scope {
public:

    const Scope* scope;
    map<string, Variable> variables;
    bool isLast;

    Scope() {
        this->scope = nullptr;
        this->isLast = true;
    }

    Scope(const Scope *pScope) {
        this->scope = pScope;
        this->isLast = false;
    }
};

typedef struct {
    bool isFound;
    Scope* scope;
} ScopeData;

class Table {
    Scope scope;

public:
    Table() {
        this->scope = Scope();
    }

    void clear() {
        scope.variables.clear();
    }

    void push() {
        scope = Scope(scope);
    }

    void pop() {
        if (!scope.isLast) {
            this->scope = scope.scope;
        }
    }

    void put(const string& key, Value value) {
        Scope sc = findScope(key).scope;
        scope.variables[key] = genVariable(false, value);
    }

    void constant(const string& key, Value value) {
        Scope sc = findScope(key).scope;
        scope.variables[key] = genVariable(true, value);
    }

    Variable getVariable(const string& key) {
        return findScope(key).scope->variables[key];
    }

    Value get(const string& key) {
        return findScope(key).scope->variables[key].value;
    }

    bool contains(const string& key) {
        return findScope(key).isFound;
    }

    ScopeData findScope(const string& var) {
        Scope current = scope;
        do {
            if (current.variables.contains(var)) {
                return genData(true, scope);
            }
            current = current.scope;
        } while (current.isLast);

        return genData(false, current);
    }

    static ScopeData genData(bool isFound, Scope& result) {
        return ScopeData({isFound, &result});
    }

    static Variable genVariable(bool isConstant, Value value) {
        return Variable ({isConstant, value});
    }
};

