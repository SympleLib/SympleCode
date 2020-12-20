#pragma once

#include "SympleCode/Common/Node/Expression/VariableExpressionNode.h"

namespace Symple
{
	class PointerIndexExpressionNode : public ModifiableExpressionNode
	{
	private:
		const ModifiableExpressionNode* mVariable;
		const ExpressionNode* mIndex;
	public:
		PointerIndexExpressionNode(const VariableExpressionNode* variable, const ExpressionNode* index)
			: mVariable(variable), mIndex(index) {}

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

		const ModifiableExpressionNode* GetExpression() const
		{
			return mVariable;
		}

		const ExpressionNode* GetIndex() const
		{
			return mIndex;
		}
	};
}