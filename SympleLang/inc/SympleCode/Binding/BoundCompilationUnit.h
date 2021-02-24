#pragma once

#include <vector>

#include "SympleCode/Syntax/TranslationUnitSyntax.h"

#include "SympleCode/Binding/Node.h"
#include "SympleCode/Binding/BoundBlockStatement.h"

#include "SympleCode/Symbol/FunctionSymbol.h"
#include "SympleCode/Symbol/StructTypeSymbol.h"

namespace Symple::Binding
{
	typedef std::vector<shared_ptr<Symbol::StructTypeSymbol>> StructMap;
	typedef std::vector<std::pair<shared_ptr<Symbol::FunctionSymbol>, shared_ptr<BoundStatement>>> FunctionMap;

	inline shared_ptr<Symbol::StructTypeSymbol> FindStruct(StructMap structs, std::string_view name)
	{
		for (auto s : structs)
			if (s->GetName() == name)
				return s;
		return nullptr;
	}

	inline shared_ptr<Symbol::FunctionSymbol> FindFunction(FunctionMap funcs, std::string_view name)
	{
		for (auto fn : funcs)
			if (fn.first->GetName() == name)
				return fn.first;
		return nullptr;
	}

	class BoundCompilationUnit : public Node
	{
	private:
		StructMap mStructures;
		FunctionMap mFunctions;
	public:
		BoundCompilationUnit(shared_ptr<Syntax::Node> syntax, StructMap structs, FunctionMap funcs)
			: Node(syntax), mStructures(structs), mFunctions(funcs) {}

		virtual Kind GetKind() override
		{ return CompilationUnit; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			for (auto &s : GetStructures())
			{ os << '\n'; s->Print(os, newIndent, true, "[Struct] "); }

			for (auto &func : GetFunctions())
			{
				os.put('\n'); func.first->Print(os, newIndent, !func.second && func == GetFunctions().back(), "[Function] ");
				if (func.second)
				{ os.put('\n'); func.second->Print(os, newIndent, func == GetFunctions().back(), "Body = "); }
			}
		}

		StructMap GetStructures()
		{ return mStructures; }

		FunctionMap GetFunctions()
		{ return mFunctions; }
	};
}