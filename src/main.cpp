#include <iostream>

#include "syc/parse/Lexer.h"
#include "syc/parse/Parser.h"
#include "syc/SourceFile.h"


int main() {
	static_assert(sizeof(size_t) == sizeof(uint64_t), "x64 only, buddy");

	syc::SourceFileId file = syc::SourceFile::ReadFile("../samples/test.sy");

	syc::Lexer lexer;
	std::vector<syc::Token> tokens = lexer.lex(file);

	if (false)
		for (const syc::Token &token: tokens)
			std::cout << "[" << token.sourceRange.start.line << ":" << token.sourceRange.start.column << "] \"" << token.text << "\" " << token.getName() << " | " << token.getFlagsAsString() << "\n";

	syc::Parser parser;
	std::vector<syc::AstNode *> ast = parser.Parse(tokens);
	if (true)
		for (const syc::AstNode *node: ast) {
			node->print(std::cout, "", "", node == ast.back());
		}

    return 0;
}
