#include "SympleCode/Syntax/Expression/ExpressionNode.h"

namespace Symple
{
	ExpressionNode::ExpressionNode(Kind kind, std::shared_ptr<Token> tok)
		: Node(kind, tok)
	{}

	ExpressionNode::ExpressionNode(std::shared_ptr<Token> tok)
		: Node(Expression, tok)
	{}
}