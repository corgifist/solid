// This script is part of SOLID language.
// Copyright 2021.

#include "vm.h"
#include "table.h"
#include "../chunk/allocations.h"

VM vm;

void initVM(Chunk *chunk) {
    CallFrame frame;
    frame.function = chunk;
    frame.stage = 0;
    vm.frames.push(frame);
    vm.table = Table();
    resetStack();
}

void freeVM() {
    for (int i = 0; i < vm.frames.size(); i++) {
        freeChunk(vm.frames.top().function);
        vm.frames.pop();
    };
}

void traceVM() {
    printf("[");
    for (int i = 0; i < vm.frames.size(); i++) {
        printf("%s, ", object_to_string(vm.frames.top().st.top()).c_str());
        vm.frames.top().st.pop();
    }
    printf("]\n");
}

void newSwitch() {
    unordered_map<string, int> newMap;
    vm.switchStack.push(newMap);
}

void pushSwitch(const Value value, int jump) {
    unordered_map<string, int> acc = vm.switchStack.top();
    vm.switchStack.pop();
    acc[object_to_string(value)] = jump;
    vm.switchStack.push(acc);
}

unordered_map<string, int> popSwitch() {
    unordered_map<string, int> result = vm.switchStack.top();
    vm.switchStack.pop();
    return result;
}

InterpretResult interpret() {

#define READ_BYTE() (frame.function->code[frame.stage++])
#define READ_CONSTANT() (frame.function->constants.values[READ_BYTE()])
#define READ_STRING() object_to_string(READ_CONSTANT())
#define READ_LINE() (getLine(frame.function, frame.stage))
#define RUNTIME_ERROR() runtime_result = RUNTIME_ERROR
#define RUNTIME_OK() runtime_result = RUNTIME_OK
#define CONSUME_EXPR(value, type_) (value.type == type_)
#define snt(strin) string(strin)

    CallFrame frame = vm.frames.top();

    for (;;) {
        runtime_check();
        switch (READ_BYTE()) {
            case DECLARE_FUNCTION: {
                ObjFunction* function = (ObjFunction*) pop().as.strange;
                print("successs");
                break;
            }
            case BEGIN_SWITCH: {
                newSwitch();
                break;
            }
            case BUILD_SWITCH: {
                Value expr = pop();
                int jump = EXACT_OPERAND(READ_CONSTANT());
                pushSwitch(expr, jump);
                break;
            }
            case SWITCH_TABLE: {
                Value source = pop();
                unordered_map<string , int> clauses = popSwitch();
                bool ok = false;
                for (const auto&[key, value] : clauses) {
                    if (key == object_to_string(source)) {
                        frame.stage = value;
                        ok = true;
                        continue;
                    }
                }
                if (!ok) {
                    for (const auto& [key, value] : clauses) {
                        if (key == "DEFAULT_CLAUSE") {
                            frame.stage = value;
                            break;
                        }
                    }
                }
                break;
            }
            case ASSIGN: {
                Value expression = pop();
                push(expression);
                string name = READ_STRING();
                if (!vm.table.contains(name)) {
                    barley_exception("UndefinedVariable", snt("undefined variable '") + name + "' to assign", READ_LINE());
                    RUNTIME_ERROR();
                    runtime_check();
                }

                if (vm.table.isConstant(name)) {
                    barley_exception("ConstantModifier", snt("unable to assign value to variable with constant modifier"), READ_LINE());
                    RUNTIME_ERROR();
                    runtime_check();
                }

                ValueType sourceType = vm.table.get(name).type;
                if (!CONSUME_EXPR(expression, sourceType)) {
                    barley_exception("TypeMismatch", snt("compatible types '") + type(expression) + "' and '" + type(vm.table.get(name)) + "'", READ_LINE());
                    RUNTIME_ERROR();
                    runtime_check();
                }

                vm.table.assign(name, expression);
                break;
            }
            case CONSTANTIFY: {
                vm.constant = true;
                break;
            }
            case LOOP: {
                uint32_t offset = EXACT_OPERAND(READ_CONSTANT());
                frame.stage = offset;
                break;
            }
            case JUMP_ANYWAY: {
                uint16_t offset = EXACT_OPERAND(READ_CONSTANT());
                frame.stage = offset;
                break;
            }
            case JUMP_IF_FALSE: {
                uint16_t offset = EXACT_OPERAND(READ_CONSTANT());
                if (isFalse(pop())) frame.stage = offset;
                break;
            }
            case PRINT: {
                print(object_to_string(pop()));
                break;
            }
            case CAST: {
                Value expression = pop();
                const char* type = READ_STRING().c_str();
                if (CONSUME_EXPR(expression, STRING)) {
                    if (strcmp(type, "r_chr8") == 0) {
                        push(CHAR(object_to_string(expression).at(0)));
                    }
                    break;
                }
                double operand = EXACT_OPERAND(expression);
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
                } else if (strcmp(type, "r_chr8") == 0) {
                    push(CHAR((char) operand));
                } else {
                    barley_exception("UndefinedType", snt("undefined type '") + snt(type) + snt("'"), READ_LINE());
                }
                break;
            }
            case DECLARE_AUTO: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
                break;
            }
            case DECLARE_R_CHR_8: {
                Value expression = pop();
                string name = READ_STRING();
                if (vm.table.contains(name)) {
                    barley_exception("DuplicateVariable", snt("variable '") + name + "' is already exists", READ_LINE());
                    runtime_check();
                }
                if (!CONSUME_EXPR(expression, CHAR)) {
                    barley_exception("TypeMismatch", "excepted u_int64 in runtime", READ_LINE());
                    runtime_check();
                }
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
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
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
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
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
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
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
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
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
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
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
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
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
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
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
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
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
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
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
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
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
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
                if (vm.constant) {
                    vm.constant = false;
                    vm.table.constant(name, expression);
                }
                else vm.table.put(name, expression);
                break;
            }
            case DUP: push(peek(0)); break;
            case POP: pop(); break;
            case SCOPE_START: vm.table.push(); break;
            case SCOPE_END: vm.table.pop(); break;
            case LONG_CONSTANT: {
                uint32_t index = READ_BYTE() |
                                 READ_BYTE() << 8 |
                                 READ_BYTE() << 16;
                Value constant = frame.function->constants.values[index];
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
                    string as = snt(a.as.string);
                    string bs = snt(b.as.string);
                    switch (op) {
                        case '+': concat(a, b); break;
                        case '!': push(BOOL(as != bs)); break;
                        case '=': push(BOOL(as == bs)); break;
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
                    case '>': push(BOOL(operandA > operandB)); break;
                    case '<': push(BOOL(operandA < operandB)); break;
                    case '=': push(BOOL(operandA == operandB)); break;
                    case 'g': push(BOOL(operandA >= operandB)); break;
                    case 'l': push(BOOL(operandA <= operandB)); break;
                    case '!': push(BOOL(operandA != operandB)); break;
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
                }

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
    for (int i = 0; i < vm.frames.top().st.size(); i++) {
        vm.frames.top().st.pop();
    }
}

void push(Value value) {
    vm.frames.top().st.push(value);
}

Value pop() {
    Value result = vm.frames.top().st.top();
    vm.frames.top().st.pop();
    return result;
}

Value peek(int relative) {
    return vm.frames.top().st.top();
}

bool isFalse(Value value) {
    return !value.as.boolean;
}