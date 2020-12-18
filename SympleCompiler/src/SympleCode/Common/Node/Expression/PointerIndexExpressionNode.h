#pragma once

#include "SympleCode/Common/Node/Expression/VariableExpressionNode.h"

namespace Symple
{
	class PointerIndexExpressionNode : public ExpressionNode
	{
	private:
		const VariableExpressionNode* mVariable;
		int mIndex;
	public:
		PointerIndexExpressionNode(const VariableExpressionNode* variable, int index)
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
			ss << "Pointer Index Expression [" << mIndex << "]";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mVariable->ToString(indent + newIndent);

			return ss.str();
		}

		const VariableExpressionNode* GetExpression() const
		{
			return mVariable;
		}

		int GetIndex() const
		{
			return mIndex;
		}
	};
}