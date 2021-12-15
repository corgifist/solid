// This script is part of SOLID language.
// Copyright 2021.

#include "vm.h"
#include "table.h"

#define READ_BYTE() (*vm.stage++)
#define READ_CONSTANT() (vm.chunk->constants.values[READ_BYTE()])
#define READ_STRING() object_to_string(READ_CONSTANT())
#define READ_LINE() (getLine(vm.chunk, (int) *vm.stage))
#define RUNTIME_ERROR() runtime_result = RUNTIME_ERROR
#define RUNTIME_OK() runtime_result = RUNTIME_OK

VM vm;

void initVM(Chunk *chunk) {
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
        runtime_check();
        switch (READ_BYTE()) {
            case LONG_CONSTANT: {
                push(SHORT(0));
                break;
            }
            case EXTRACT_BIND: {
                push(Table::get(READ_STRING()));
                break;
            }
            case BINARY: {
                Value b = pop();
                Value a = pop();
                char op = READ_BYTE();
                if (!(IS_NUMBER_SUBSET(a) && IS_NUMBER_SUBSET(b))) {
                    string acc = "caught error while evaluating '";
                    acc += object_to_string(a);
                    acc += " ";
                    acc += op;
                    acc += " ";
                    acc += object_to_string(b);
                    acc += "'";
                    barley_exception("BadOperand", acc, READ_LINE());
                    RUNTIME_ERROR();
                }
                double operandA = EXACT_OPERAND(a);
                double operandB = EXACT_OPERAND(b);
                switch (op) {
                    case '+': push(NUMBER_FROM_SOURCE(a, operandA + operandB)); break;
                    case '-': push(NUMBER_FROM_SOURCE(a, operandA - operandB)); break;
                    case '*': push(NUMBER_FROM_SOURCE(a, operandA * operandB)); break;
                    case '/': push(NUMBER_FROM_SOURCE(a, operandA / operandB)); break;
                }
                break;
            }
            case UNARY: {
                Value operand = pop();
                char op = READ_BYTE();
                if (!IS_NUMBER_SUBSET(operand)) {
                    string acc = "expected number as operand in '-";
                    acc += object_to_string(operand);
                    acc += "'";
                    barley_exception("BadOperand", acc, READ_LINE());
                    RUNTIME_ERROR();
                };

                double value = EXACT_OPERAND(operand);

                switch (op) {
                    case '-':
                        push(NUMBER_FROM_SOURCE(operand, -value));
                        break;
                    default: {
                        string acc = "this is a bug. unsupported operation '";
                        acc += op;
                        acc += "'";
                        barley_exception("VMException", acc, READ_LINE());
                        RUNTIME_ERROR();
                    }
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
                RUNTIME_OK();
                return RUNTIME_OK;
        }
    }
}

void resetStack() {
    vm.stackCount = 0;
}

void push(Value value) {
    if (vm.stackCapacity < vm.stackCount + 1) {
        int oldCapacity = vm.stackCapacity;
        vm.stackCapacity = GROW_CAPACITY(oldCapacity);
        vm.stack = GROW_ARRAY(Value, vm.stack, vm.stackCount, vm.stackCapacity);
    }

    vm.stack[vm.stackCount] = value;
    vm.stackCount++;
}

Value pop() {
    vm.stackCount--;
    return vm.stack[vm.stackCount];
}