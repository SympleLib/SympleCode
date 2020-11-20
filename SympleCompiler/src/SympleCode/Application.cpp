#include <iomanip>
#include <iostream>

#include "SympleCode/Lexer.hpp"
#include "SympleCode/Parser.hpp"

int main() {
	std::string source;
	while (true)
	{
		std::getline(std::cin, source);

		//Symple::Lexer::Lex(source.c_str(), [](const Symple::TokenInfo& tok)
		//	{
		//		std::cout << Symple::Tokens::ToString(tok.GetToken()) << " |" << tok.GetLex() << "|\n";

		//		return !tok.IsEither(Symple::Tokens::End, Symple::Tokens::Unexpected);
		//	});
		Symple::Parser::Parse(source);
		source.clear();
	}

	//std::cin.get();
}