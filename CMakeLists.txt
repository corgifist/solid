cmake_minimum_required(VERSION 3.17)
project(Solid)

set(CMAKE_CXX_STANDARD 20)

add_executable(Solid src/solid.cpp src/utils.h src/runtime.h src/common.h src/values/type.h src/runtime/table.cpp src/runtime/table.h src/parser/compiler.cpp src/parser/compiler.h src/parser/lexer.h src/parser/lexer.cpp src/parser/parser.h src/parser/parser.cpp src/chunk/allocations.cpp src/chunk/allocations.h src/chunk/chunk.cpp src/chunk/chunk.h src/chunk/constants.cpp src/chunk/constants.h src/chunk/debug.cpp src/chunk/constants.cpp src/runtime/table.h src/runtime/table.cpp src/runtime/vm.h src/runtime/vm.cpp)