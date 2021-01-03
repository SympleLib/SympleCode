#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Variable/Variable.h"
#include "SympleCode/Common/Node/Expression/ModifiableExpressionNode.h"

#include "SympleCode/Common/Analysis/Debug.h"

namespace Symple
{
	class VariableExpressionNode : public ModifiableExpressionNode, public Variable
	{
	private:
		const Token* mName;
	public:
		VariableExpressionNode(const Token* name)
			: ModifiableExpressionNode(Debug::GetVariable(name->GetLex())->GetType()), mName(name) {}

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