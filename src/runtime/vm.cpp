// This script is part of SOLID language.
// Copyright 2021.

#include "vm.h"
#include<strings.h>

#define READ_BYTE() (*vm.stage++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_LINE() (vm.chunk->lines[*vm.stage])
#define RUNTIME_ERROR() runtime_result = RUNTIME_ERROR

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
    for (;;) {
        uint8_t instruction;
        runtime_check();
        switch (instruction = READ_BYTE()) {
            case UNARY: {
                char op = READ_BYTE();
                Value operand = pop();
                if (!IS_NUMBER(operand)) {
                    string acc = "expected number as operand in '-";
                    acc += object_to_string(operand);
                    acc += "'";
                    barley_exception("BadOperand", acc, READ_LINE());
                    RUNTIME_ERROR();
                };

                switch (op) {
                    case '-': push(NUMBER(-AS_NUMBER(operand)));
                }
                break;
            }
            case CONSTANT: {
                Value constant = READ_CONSTANT();
                push(constant);
                break;
            }
            case RETURN:
                print(object_to_string(pop()));
                runtime_result = RUNTIME_OK;
                return RUNTIME_OK;
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