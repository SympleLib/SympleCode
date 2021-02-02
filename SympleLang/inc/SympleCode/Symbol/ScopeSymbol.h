#pragma once

#include "SympleCode/Syntax/BlockStatementSyntax.h"

#include "SympleCode/Binding/BoundStatement.h"
#include "SympleCode/Binding/Type.h"

namespace Symple::Binding
{
	class BoundScope : public BoundStatement
	{
	private:
	public:
		BoundScope(shared_ptr<Syntax::BlockStatementSyntax> syntax, shared_ptr<Type> ty)
			: BoundStatement(syntax), mType(ty)
		{}

		virtual Kind GetKind() override
		{ return Function; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			os << " '"; GetType()->PrintShort(os); os << ' ' << GetName()->GetText(); os.put('(');
			for (auto param : GetParameters())
			{
				param->GetType()->PrintShort(os);
				if (param != GetParameters().back())
					os << ", ";
			}
			os << ")'";

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);
		}
	};
}