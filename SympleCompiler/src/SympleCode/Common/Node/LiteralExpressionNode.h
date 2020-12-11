#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/ExpressionNode.h"

namespace Symple
{
	class LiteralExpressionNode : public ExpressionNode
	{
	protected:
		const Token* mLiteral;
	public:
		LiteralExpressionNode(const Token* literal)
			: mLiteral(literal) {}

		virtual Kind GetKind() const override
		{
			return Kind::LiteralExpression;
		}

		virtual std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Literal Expression (" << mLiteral->GetLex() << ")";

			return ss.str();
		}

        const Token* GetLiteral() const { return mLiteral; }
	};
}