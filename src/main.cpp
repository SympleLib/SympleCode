#include <iostream>

#include "syc/Lexer.h"
#include "syc/Parser.h"
#include "syc/SourceFile.h"

int main() {
	syc::SourceFileId file = syc::SourceFile::ReadFile("../samples/test.sy");

	syc::Lexer lexer;
	std::vector<syc::Token> tokens = lexer.lex(file);

	if (false)
	for (const syc::Token &token: tokens)
		std::cout << "[" << token.sourceRange.start.line << ":" << token.sourceRange.start.column << "] \"" << token.text << "\" " << token.getName() << " | " << token.getFlagsAsString() << "\n";

	syc::Parser parser;
	std::vector<syc::AstNode *> ast = parser.Parse(tokens);


    return 0;
}
