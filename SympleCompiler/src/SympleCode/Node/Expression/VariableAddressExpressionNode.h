#pragma once

#include "SympleCode/Node/Expression/Modifiable/VariableExpressionNode.h"

namespace Symple
{
	class VariableAddressExpressionNode : public ExpressionNode
	{
	private:
		const Token* mSymbol;
		const VariableExpressionNode* mVariable;
	public:
		VariableAddressExpressionNode(const Token* symbol, const VariableExpressionNode* variable)
			: ExpressionNode(new TypeNode(variable->GetType()->GetType(), MutModifiers,
				new TypeContinueNode(PtrToken, variable->GetType()->GetModifiers(), variable->GetType()->GetContinue()))), mSymbol(symbol), mVariable(variable) {}

		Kind GetKind() const override
		{
			return Kind::VariableAddressExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Variable Address Expression";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mVariable->ToString(indent + newIndent);
			return ss.str();
		}

		const Token* GetSymbol() const
		{
			return mSymbol;
		}

		const VariableExpressionNode* GetVariable() const
		{
			return mVariable;
		}
	};
}