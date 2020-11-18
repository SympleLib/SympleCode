#include <iomanip>
#include <iostream>

#include "SympleCode/Parser.hpp"

int main() {
    const char* source = R"(
print(1 + 1);
    )";

    Symple::Parser::Parse(source);

    std::cin.get();
}