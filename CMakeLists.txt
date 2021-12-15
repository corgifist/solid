cmake_minimum_required(VERSION 3.21)
project(Solid)

set(CMAKE_CXX_STANDARD 20)

add_executable(Solid src/solid.cpp src/chunk/chunk.cpp src/chunk/chunk.h src/chunk/allocations.h src/chunk/allocations.cpp src/chunk/debug.h src/chunk/debug.cpp src/values/type.h src/chunk/constnants.h src/chunk/constants.cpp src/runtime/vm.h src/runtime/vm.cpp src/runtime.h src/runtime/table.h src/runtime/table.cpp src/parser/parser.h src/parser/parser.cpp src/parser/compiler.h src/parser/compiler.cpp)
