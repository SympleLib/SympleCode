#include "SympleCode/Syntax/Expression/LiteralExpressionNode.h"

namespace Symple
{
	LiteralExpressionNode::LiteralExpressionNode(std::shared_ptr<Token> tok)
		: ExpressionNode(LiteralExpression, tok)
	{}
}