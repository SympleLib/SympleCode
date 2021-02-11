#pragma once

#include "SympleCode/Syntax/StatementSyntax.h"

namespace Symple::Syntax
{
	class NativeStatementSyntax : public StatementSyntax
	{
	private:
		shared_ptr<Token> mAssembly;
	public:
		NativeStatementSyntax(shared_ptr<Token> key, shared_ptr<Token> code)
			: StatementSyntax(key), mAssembly(code) {}

		virtual Kind GetKind() override
		{ return NativeStatement; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetAssembly()->Print(os, newIndent, true, "Assembly = ");
		}

		shared_ptr<Token> GetKeyword()
		{ return GetToken(); }

		shared_ptr<Token> GetAssembly()
		{ return mAssembly; }
	};
}