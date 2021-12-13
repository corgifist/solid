// This script is part of SOLID language.
// Copyright 2021.

#include "vm.h"

#define READ_BYTE() (*vm.stage++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])

VM vm;

void initVM(Chunk* chunk) {
    vm.chunk = chunk;
    vm.stage = vm.chunk->code;
    resetStack();
}

void freeVM() {
    freeChunk(vm.chunk);
    initVM(vm.chunk);
}

InterpretResult interpret() {
    runtime_result = RUNTIME_ERROR;
    for (;;) {
        uint8_t instruction;
        runtime_check();
        if (runtime_result == RUNTIME_ERROR) break;
        switch (instruction = READ_BYTE()) {
            case RETURN:
                print("return");
                runtime_result = RUNTIME_OK;
                return RUNTIME_OK;
            case CONST:
                Value constant = READ_CONSTANT();
                print(object_to_string(constant));
                break;

        }
    }
}

void resetStack() {
    vm.stackTop = vm.stack;
}

void push(Value value) {
    *vm.stackTop = value;
    vm.stackTop++;
}

Value pop() {
    vm.stackTop--;
    return *vm.stackTop;
}