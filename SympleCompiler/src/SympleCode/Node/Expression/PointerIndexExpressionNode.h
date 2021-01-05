#pragma once

#include "SympleCode/Node/Expression/ModifiableExpressionNode.h"

namespace Symple
{
	class PointerIndexExpressionNode : public ModifiableExpressionNode
	{
	private:
		const ExpressionNode* mVariable;
		const ExpressionNode* mIndex;
	public:
		PointerIndexExpressionNode(const ExpressionNode* variable, const ExpressionNode* index)
			: ModifiableExpressionNode(variable->GetType()), mVariable(variable), mIndex(index) {}

		Kind GetKind() const override
		{
			return Kind::PointerIndexExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Pointer Index Expression";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mVariable->ToString(indent + newIndent, false);
			ss << '\n' << mIndex->ToString(indent + newIndent);

			return ss.str();
		}

		const ExpressionNode* GetExpression() const
		{
			return mVariable;
		}

		const ExpressionNode* GetIndex() const
		{
			return mIndex;
		}
	};
}