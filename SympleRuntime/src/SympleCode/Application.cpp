#include <iomanip>
#include <iostream>

#include "SympleCode/Lexer.hpp"

int main() {
    const char* source = R"(
var myVar = 69;
var myOtherVar = 64;
    )";

    Lex(source);
    for (auto token = Next();
        !token.IsEither(Tokens::End, Tokens::Unexpected);
        token = Next())
        std::cout << std::setw(12) << Tokens::ToString(token.GetToken()) << " |" << token.GetLex()
        << "|\n";

    std::cin.get();
}