cmake_minimum_required(VERSION 3.22)
project(SympleCode)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 23)

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_executable(SympleCode src/main.cpp src/sy/Token.h src/pch.h src/sy/Lexer.cpp src/sy/Lexer.h src/sy/Compiler.cpp src/sy/Compiler.h src/sy/Parser.cpp src/sy/Parser.h src/sy/Ast.h src/sy/Ast.cpp)
target_precompile_headers(
        SympleCode
    PUBLIC
        src/pch.h
)
target_include_directories(
        SympleCode
        PUBLIC
        src)

target_link_libraries(SympleCode ${LLVM_AVAILABLE_LIBS})