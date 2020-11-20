#include <iomanip>
#include <iostream>

#include "SympleCode/Parser.hpp"

int main() {
    std::string source;
    while (true)
    {
        std::getline(std::cin, source);

        Symple::Parser::Parse(source);
        source.clear();
    }

    //std::cin.get();
}