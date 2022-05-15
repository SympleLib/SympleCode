cmake_minimum_required(VERSION 3.22)
project(SympleCode)

#find_package(LLVM REQUIRED CONFIG)
#
#message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
#message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 23)

#include_directories(${LLVM_INCLUDE_DIRS})
#separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
#add_definitions(${LLVM_DEFINITIONS_LIST})

target_include_directories(
        SympleCode
        PUBLIC
        src)
add_executable(SympleCode src/main.cpp src/syc/Lexer.cpp src/syc/Lexer.h src/syc/Token.cpp src/syc/Token.h src/syc/SourceLocation.h src/syc/TokenKind.h src/syc/SourceFile.cpp src/syc/SourceFile.h src/syc/ast/ExprAst.cpp src/syc/ast/ExprAst.h src/syc/ast/LiteralAst.cpp src/syc/ast/LiteralAst.h src/syc/ast/AstNode.cpp src/syc/ast/AstNode.h src/syc/Parser.cpp src/syc/Parser.h src/syc/ast/Ast.h src/syc/emit/Emitter.cpp src/syc/emit/Emitter.h)

#llvm_map_components_to_libnames(llvm_libs support core irreader)
#target_link_libraries(SympleCode ${llvm_libs})