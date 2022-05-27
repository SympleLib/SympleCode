cmake_minimum_required(VERSION 3.22)
project(SympleCode)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

set(CMAKE_CXX_STANDARD 23)

include_directories(${LLVM_INCLUDE_DIRS})
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})

add_executable(SympleCode src/main.cpp src/syc/parse/Lexer.cpp src/syc/parse/Lexer.h src/syc/parse/Token.cpp src/syc/parse/Token.h src/syc/SourceLocation.h src/syc/parse/TokenKind.h src/syc/SourceFile.cpp src/syc/SourceFile.h src/syc/ast/ExprAst.cpp src/syc/ast/ExprAst.h src/syc/ast/LiteralAst.cpp src/syc/ast/LiteralAst.h src/syc/ast/AstNode.cpp src/syc/ast/AstNode.h src/syc/parse/Parser.cpp src/syc/parse/Parser.h src/syc/ast/Ast.h src/syc/ast/AstKind.h src/syc/ast/Operator.h src/syc/FunEnum.h src/syc/emit/Emitter.cpp src/syc/emit/Emitter.h src/syc/ast/StmtAst.cpp src/syc/ast/StmtAst.h src/syc/ast/TypeAst.cpp src/syc/ast/TypeAst.h src/syc/ast/TypeKind.h src/syc/parse/ParseScope.h src/syc/ast/RefTypeKind.h src/syc/ast/RefTypeKind.def)
target_include_directories(
        SympleCode
        PUBLIC
        src)

target_link_libraries(SympleCode ${LLVM_AVAILABLE_LIBS})