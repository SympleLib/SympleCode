#pragma once

#include "SympleCode/Syntax/ExpressionSyntax.h"

#include "SympleCode/Binding/Node.h"
#include "SympleCode/Binding/Type.h"

namespace Symple::Binding
{
	class BoundExpression : public Node
	{
	public:
		BoundExpression(shared_ptr<Syntax::ExpressionSyntax> syntax)
			: Node(syntax) {}

		virtual Kind GetKind()
		{ return Expression; }

		virtual shared_ptr<Type> GetType()
		{ return Type::ErrorType; }

		virtual bool IsMutable()
		{ return false; }

		virtual void Print(std::ostream & os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetType()->Print(os, newIndent, true, "Type = ");
		}
	};
}