#pragma once

#include <string_view>

#include "SympleCode/DiagnosticBag.h"
#include "SympleCode/Syntax/Token.h"
#include "SympleCode/Syntax/TranslationUnitSyntax.h"
#include "SympleCode/Binding/Binder.h"
#include "SympleCode/Binding/BoundCompilationUnit.h"
#include "SympleCode/Emit/AsmEmitter.h"

namespace Symple
{
	class __SYC_API Compiler
	{
	private:
		std::string mPath, mAsmPath;
		Syntax::TokenList mTokens;
		shared_ptr<Syntax::TranslationUnitSyntax> mAST;
		shared_ptr<Binding::BoundCompilationUnit> mTree;
		unique_ptr<Emit::AsmEmitter> mEmitter;

		bool mAnyErrors = false;
		static std::vector<std::string> sLibraries;
		static std::vector<std::string> sUnits;

		friend class Binding::Binder;
	public:
		Compiler(char *path);

		shared_ptr<DiagnosticBag> Lex();
		shared_ptr<DiagnosticBag> Parse();
		shared_ptr<DiagnosticBag> Bind();
		shared_ptr<Binding::BoundCompilationUnit> BindSymbols();
		void Emit();
		void Compile();
		// Returns true if links successfully
		bool Link(std::string_view output = "sy/bin/Main.exe", bool isLibrary = false);
		int Exec(std::string_view args = "");

		/// <summary>
		/// Print Diagnostics
		/// </summary>
		/// <param name="diagnostics">Diagnostic Bag</param>
		/// <param name="step">Step in witch diagnostics are for</param>
		/// <returns>Returns true if there is an error</returns>
		bool PrintDiagnosticBag(shared_ptr<DiagnosticBag> diagnostics, char *step = "Null Step");
	};
}