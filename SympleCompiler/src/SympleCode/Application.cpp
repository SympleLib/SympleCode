#include <iomanip>
#include <iostream>

#include "SympleCode/Parser.hpp"

int main() {
    const char* source = R"(
int a;
a = 69;
    )";

    Symple::Parser::Parse(source);

    std::cin.get();
}