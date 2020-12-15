#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/ExpressionNode.h"

namespace Symple
{
	class VariableExpressionNode : public ExpressionNode
	{
	private:
		const Token* mName;
	public:
		VariableExpressionNode(const Token* name)
			: mName(name) {}

		Kind GetKind() const override
		{
			return Kind::VariableExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Variable Expression " << mName->GetLex();

			return ss.str();
		}

		const Token* GetName() const
		{
			return mName;
		}
	};
}