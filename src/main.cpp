#include <iostream>

#include "syc/parse/Lexer.h"
#include "syc/parse/Parser.h"
#include "syc/emit/Emitter.h"
#include "syc/SourceFile.h"
#include "syc/emit/SycJIT.h"

#include <llvm/Support/TargetSelect.h>
#include <llvm/Target/TargetMachine.h>

int main() {
	static_assert(sizeof(size_t) == sizeof(uint64_t), "x64 only, buddy");

	syc::SourceFileId file = syc::SourceFile::ReadFile("../samples/test.sy");

	syc::Lexer lexer;
	std::vector<syc::Token> tokens = lexer.lex(file);

	if (true)
		for (const syc::Token &token: tokens)
			std::cout << "[" << token.sourceRange.start.line << ":" << token.sourceRange.start.column << "] \"" << token.text << "\" " << token.getName() << " | " << token.getFlagsAsString() << "\n";

	syc::Parser parser;
	std::vector<syc::AstNode *> ast = parser.Parse(tokens);
	if (true)
		for (const syc::AstNode *node: ast) {
			node->print(std::cout, "", "", node == ast.back());
		}

	if (true) {
		llvm::LLVMContext ctx;
		syc::emit::Emitter emitter(ctx);
		llvm::Module *module = emitter.Emit(ast, "samples/test.sy");
		if (true) {
			std::string str;
			llvm::raw_string_ostream os(str);
			module->print(os, nullptr);
			std::cout << str;
		}

		if (true) {
			llvm::InitializeNativeTarget();
			llvm::InitializeNativeTargetAsmParser();
			llvm::InitializeNativeTargetAsmParser();

			std::unique_ptr<syc::emit::SycJIT> jit = std::move(*syc::emit::SycJIT::Create());

		}
	}
}
