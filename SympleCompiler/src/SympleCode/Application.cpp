#include <iomanip>
#include <iostream>

#include "SympleCode/Parser.hpp"

int main() {
    const char* source = R"(
int a = 22;
    )";

    Symple::Parser::Parse(source);

    //std::cin.get();
}