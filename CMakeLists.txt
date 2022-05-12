cmake_minimum_required(VERSION 3.22)
project(SympleCode)

set(CMAKE_CXX_STANDARD 23)

add_executable(SympleCode src/main.cpp src/syc/Lexer.cpp src/syc/Lexer.h src/syc/Token.cpp src/syc/Token.h src/syc/SourceLocation.h src/syc/TokenKind.h src/syc/SourceFile.cpp src/syc/SourceFile.h src/syc/ast/ExprAst.cpp src/syc/ast/ExprAst.h src/syc/ast/LiteralAst.cpp src/syc/ast/LiteralAst.h src/syc/ast/AstNode.cpp src/syc/ast/AstNode.h src/syc/Parser.cpp src/syc/Parser.h src/syc/ast/Ast.h)

target_include_directories(
        SympleCode
        PUBLIC
        src)