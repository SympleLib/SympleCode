cmake_minimum_required(VERSION 3.22)
project(SympleCode)

set(CMAKE_CXX_STANDARD 23)

add_executable(SympleCode src/main.cpp src/Symple/Lexer.cpp src/Symple/Lexer.h src/Symple/Token.cpp src/Symple/Token.h src/Symple/SourceRange.h src/Symple/TokenKind.h)

target_include_directories(
        SympleCode
        PUBLIC
        src)