#include <iostream>

#include "syc/parse/Lexer.h"
#include "syc/parse/Parser.h"
#include "syc/emit/Emitter.h"
#include "syc/SourceFile.h"

#include <llvm/IR/LegacyPassManager.h>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Support/Host.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>

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
			std::cout << str << '\n';
		}

		if (true) {
			llvm::InitializeAllTargetInfos();
			llvm::InitializeAllTargets();
			llvm::InitializeAllTargetMCs();
			llvm::InitializeAllAsmParsers();
			llvm::InitializeAllAsmPrinters();

			std::string targetTriple = llvm::sys::getDefaultTargetTriple();

			std::string err;
			const llvm::Target *target = llvm::TargetRegistry::lookupTarget(targetTriple, err);

			if (!target) {
				std::cerr << err;
				return EXIT_FAILURE;
			}

			llvm::TargetOptions opts;
			llvm::Optional<llvm::Reloc::Model> rm;
			llvm::TargetMachine *machine = target->createTargetMachine(targetTriple, "generic", "", opts, rm);

			module->setDataLayout(machine->createDataLayout());
			module->setTargetTriple(targetTriple);

			std::error_code ec;
			llvm::raw_fd_ostream dest("../samples/test.o", ec, llvm::sys::fs::OF_None);

			if (ec) {
				std::cerr << "can't open obj file: " << ec.message();
				return EXIT_FAILURE;
			}

			llvm::legacy::PassManager pass;
			machine->addPassesToEmitFile(pass, dest, nullptr, llvm::CGFT_ObjectFile);
			pass.run(*module);
			dest.flush();

			if (true) {
				if (!system("clang \"../samples/test.o\" -o \"../samples/test.exe\"")) {
					int result = system("\"../samples/test.exe\"");
					std::cout << "we got: " << result << '\n';
				}
			}
		}
	}
}
