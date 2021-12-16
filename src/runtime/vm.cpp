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
#define CONSUME_EXPR(value, type_) (value.type == type_)
#define snt(strin) string(strin)

VM vm;

void initVM(Chunk *chunk) {
    vm.chunk = chunk;
    vm.stage = vm.chunk->code;
    vm.table = Table();
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
            case PRINT: {
                print(object_to_string(pop()));
                break;
            }
            case CAST: {
                Value expression = pop();
                double operand = EXACT_OPERAND(expression);
                const char* type = READ_STRING().c_str();
                if (strcmp(type, "r_shrt16") == 0) {
                    push(SHORT((short) operand));
                } else if (strcmp(type, "r_int32") == 0) {
                    push(INT((int) operand));
                } else if (strcmp(type, "r_int64") == 0) {
                    push(LONG((long) operand));
                } else if (strcmp(type, "r_float64") == 0) {
                    push(DOUBLE(operand));
                } else if (strcmp(type, "r_byte8") == 0) {
                    push(BYTE((char) operand));
                } else if (strcmp(type, "u_byte8") == 0) {
                    push(UNSIGNED_BYTE((unsigned char) operand));
                } else if (strcmp(type, "u_shrt16") == 0) {
                    push(UNSIGNED_SHORT((unsigned short) operand));
                } else if (strcmp(type, "u_int32") == 0) {
                    push(UNSIGNED_INT((unsigned int) operand));
                } else if (strcmp(type, "u_int64") == 0) {
                    push(UNSIGNED_LONG((unsigned long) operand));
                } else {
                    barley_exception("UndefinedType", snt("undefined type '") + snt(type) + snt("'"), READ_LINE());
                }
                break;
            }
            case DECLARE_U_INT_64: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (!CONSUME_EXPR(expression, UNSIGNED_LONG)) {
                    barley_exception("TypeMismatch", "excepted u_int64 in runtime", READ_LINE());
                    runtime_check();
                }
                vm.table.put(name, expression);
                break;
            }
            case DECLARE_U_INT_32: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (!CONSUME_EXPR(expression, UNSIGNED_INT)) {
                    barley_exception("TypeMismatch", "excepted u_int32 in runtime", READ_LINE());
                    runtime_check();
                }
                vm.table.put(name, expression);
                break;
            }
            case DECLARE_U_SHRT_16: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (!CONSUME_EXPR(expression, UNSIGNED_SHORT)) {
                    barley_exception("TypeMismatch", "excepted u_shrt16 in runtime", READ_LINE());
                    runtime_check();
                }
                vm.table.put(name, expression);
                break;
            }
            case DECLARE_U_BYTE_8: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (!CONSUME_EXPR(expression, UNSIGNED_BYTE)) {
                    barley_exception("TypeMismatch", "excepted u_byte8 in runtime", READ_LINE());
                    runtime_check();
                }
                vm.table.put(name, expression);
                break;
            }
            case DECLARE_R_BYTE_8: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (!CONSUME_EXPR(expression, BYTE)) {
                    barley_exception("TypeMismatch", "excepted r_byte8 in runtime", READ_LINE());
                    runtime_check();
                }
                vm.table.put(name, expression);
                break;
            }
            case DECLARE_R_BOOL_1: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (!CONSUME_EXPR(expression, BOOL)) {
                    barley_exception("TypeMismatch", "excepted r_bool1 in runtime", READ_LINE());
                    runtime_check();
                }
                vm.table.put(name, expression);
                break;
            }
            case DECLARE_R_CHR_PTR: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (!CONSUME_EXPR(expression, STRING)) {
                    barley_exception("TypeMismatch", "excepted r_chr_ptr in runtime", READ_LINE());
                    runtime_check();
                }
                vm.table.put(name, expression);
                break;
            }
            case DECLARE_R_FLOAT_64: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (!CONSUME_EXPR(expression, DOUBLE)) {
                    barley_exception("TypeMismatch", "excepted r_float64 in runtime", READ_LINE());
                    runtime_check();
                }
                vm.table.put(name, expression);
                break;
            }
            case DECLARE_R_INT_64: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (!CONSUME_EXPR(expression, LONG)) {
                    barley_exception("TypeMismatch", "excepted r_int64 in runtime", READ_LINE());
                    runtime_check();
                }
                vm.table.put(name, expression);
                break;
            }
            case DECLARE_R_INT_16: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (!CONSUME_EXPR(expression, SHORT)) {
                    barley_exception("TypeMismatch", "excepted short in runtime", READ_LINE());
                    runtime_check();
                }
                vm.table.put(name, expression);
                break;
            }
            case DECLARE_R_INT_32: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (!CONSUME_EXPR(expression, INT)) {
                    barley_exception("TypeMismatch", "excepted int in r_int32 declaration", READ_LINE());
                    runtime_check();
                }
                vm.table.put(name, expression);
                break;
            }
            case POP: pop(); break;
            case SCOPE_START: vm.table.push(); break;
            case SCOPE_END: vm.table.pop(); break;
            case LONG_CONSTANT: {
                uint32_t index = vm.chunk->code[READ_BYTE()] |
                                 (vm.chunk->code[READ_BYTE()] << 8) |
                                 (vm.chunk->code[READ_BYTE()] << 16);
                Value constant = vm.chunk->constants.values[index];
                push(constant);
                break;
            }
            case EXTRACT_BIND: {
                string var = READ_STRING();
                if (!vm.table.contains(var)) {
                    barley_exception("UndefinedVar", snt("undefined variable '") + var + "'", READ_LINE());
                    runtime_check();
                }
                push(vm.table.get(var));
                break;
            }
            case BINARY: {
                Value b = pop();
                Value a = pop();
                char op = READ_BYTE();
                if (CONSUME_EXPR(a, STRING) || CONSUME_EXPR(b, STRING)) {
                    switch (op) {
                        case '+': concat(a, b); break;
                        default:
                            barley_exception("BadOperand", snt("unsupported operation '") + op + "' for strings", READ_LINE());
                            break;
                    }
                    break;
                }
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
                RUNTIME_OK();
                return RUNTIME_OK;
        }
    }
}

void concat(Value a, Value b) {
    push(STRING((snt(a.as.string) + snt(b.as.string)).c_str()));
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
    if (vm.stackCount == 0) {
        barley_exception("StackEmpty", "stack is empty", READ_LINE());
        runtime_check();
    }
    vm.stackCount--;
    return vm.stack[vm.stackCount];
}