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
		std::vector<shared_ptr<Compiler>> mUnits;

		bool mAnyErrors = false;
		static std::vector<std::string> sLibraries;

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
		bool Link();
		int Exec();

		/// <summary>
		/// Print Diagnostics
		/// </summary>
		/// <param name="diagnostics">Diagnostic Bag</param>
		/// <param name="step">Step in witch diagnostics are for</param>
		/// <returns>Returns true if there is an error</returns>
		bool PrintDiagnosticBag(shared_ptr<DiagnosticBag> diagnostics, char *step = "Null Step");
	};
}