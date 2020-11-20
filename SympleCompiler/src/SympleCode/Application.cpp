#include <iomanip>
#include <iostream>

#include "SympleCode/Lexer.hpp"
#include "SympleCode/Parser.hpp"

int main() {
    std::string source;
    while (true)
    {
        std::getline(std::cin, source);

        Symple::Lexer::Lex(source.c_str(), [](const TokenInfo& tok)
            {
                std::cout << Tokens::ToString(tok.GetToken()) << " |" << tok.GetLex() << "|\n";

                return !tok.IsEither(Tokens::End, Tokens::Unexpected);
            });
        //Symple::Parser::Parse(source);
        source.clear();
    }

    //std::cin.get();
}