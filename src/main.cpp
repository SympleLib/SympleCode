#include <iostream>

#include "Symple/Lexer.h"

namespace syc {
	using namespace Symple;
}

int main() {
	std::string src = "hello guys 69 420 1234567890lol\nolo ez3";

	syc::Lexer lexer;
	std::vector<syc::Token> tokens = lexer.lex(src);
	for (const syc::Token &token: tokens)
		std::cout << token.getName() << " \"" << token.text << "\" " << ", (" << token.sourceRange.start.line << ", " << token.sourceRange.start.column << ")\n";
    return 0;
}
