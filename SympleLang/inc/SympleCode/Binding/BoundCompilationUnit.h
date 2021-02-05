#pragma once

#include <vector>

#include "SympleCode/Syntax/TranslationUnitSyntax.h"

#include "SympleCode/Binding/Node.h"
#include "SympleCode/Binding/BoundBlockStatement.h"

#include "SympleCode/Symbol/FunctionSymbol.h"

namespace Symple::Binding
{
	typedef std::vector<std::pair<shared_ptr<Symbol::FunctionSymbol>, shared_ptr<BoundStatement>>> FunctionMap;

	class BoundCompilationUnit : public Node
	{
	private:
		FunctionMap mFunctions;
	public:
		BoundCompilationUnit(shared_ptr<Syntax::TranslationUnitSyntax> syntax, FunctionMap funcs)
			: Node(syntax), mFunctions(funcs) {}

		virtual Kind GetKind() override
		{ return CompilationUnit; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			for (auto& func : GetFunctions())
			{
				os.put('\n'); func.first->Print(os, newIndent, false, "Function = ");
				os.put('\n'); func.second->Print(os, newIndent, func == GetFunctions().back(), "Body = ");
			}
		}

		FunctionMap GetFunctions()
		{ return mFunctions; }
	};
}