#pragma once

#include "SympleCode/Syntax/Node.h"

namespace Symple
{
	class ExpressionNode : public Node
	{
	public:
		ExpressionNode(shared_ptr<Token> tok)
			: Node(tok) {}

		virtual Kind GetKind()
		{ return Expression; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			PrintIndent(os, indent, last, label);
			os << "Expression Node" << KindMap[GetKind()];
		}
	};
}