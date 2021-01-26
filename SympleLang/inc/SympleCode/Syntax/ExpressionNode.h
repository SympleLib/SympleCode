#pragma once

#include "SympleCode/Syntax/Node.h"

namespace Symple
{
	class ExpressionNode : public Node
	{
	public:
		ExpressionNode(std::shared_ptr<Token> tok)
			: Node(tok) {}

		virtual Kind GetKind()
		{ return Expression; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true)
		{
			PrintIndent(os, indent, last);
			os << "Expression Node" << KindMap[GetKind()];
		}
	};
}