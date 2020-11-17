#include <iomanip>
#include <iostream>

#include "SympleCode/Parser.hpp"

int main() {
    const char* source = R"(
5 - 5;
    )";

    Symple::Parser::Parse(source);

    std::cin.get();
}