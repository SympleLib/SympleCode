#pragma once

#include "SympleCode/Syntax/NativeStatementSyntax.h"

#include "SympleCode/Binding/BoundStatement.h"

#include "SympleCode/Symbol/FunctionSymbol.h"

namespace Symple::Binding
{
	class BoundNativeCode : public BoundStatement
	{
	public:
		BoundNativeCode(shared_ptr<Syntax::NativeStatementSyntax> syntax)
			: BoundStatement(syntax)
		{}

		virtual Kind GetKind() override
		{ return NativeCode; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
			os << '\'' << GetAssembly() << '\'';
		}

		std::string_view GetAssembly()
		{ return dynamic_pointer_cast<Syntax::NativeStatementSyntax>(GetSyntax())->GetAssembly()->GetText(); }
	};
}