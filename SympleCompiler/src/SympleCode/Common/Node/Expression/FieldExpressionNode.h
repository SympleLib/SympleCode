#pragma once

#include "SympleCode/Common/Node/Expression/VariableExpressionNode.h"
#include "SympleCode/Common/Analysis/Struct.h"

namespace Symple
{
	class FieldExpressionNode : public ExpressionNode
	{
	private:
		const VariableExpressionNode* mVariable;
		const Struct* mType;
		const Token* mField;
	public:
		FieldExpressionNode(const VariableExpressionNode* variable, const Struct* type, const Token* field)
			: mVariable(variable), mType(type), mField(field) {}

		Kind GetKind() const override
		{
			return Kind::FieldExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Field Expression [" << mType->GetName() << '.' << mField->GetLex() << "]";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mVariable->ToString(indent + newIndent, false);

			return ss.str();
		}

		const Token* GetField() const { return mField; }

		const Struct* GetType() const { return mType; }

		const VariableExpressionNode* GetRight() const { return mVariable; }
	};
}