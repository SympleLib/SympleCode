#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Function/Function.h"
#include "SympleCode/Node/Statement/VariableDeclarationNode.h"

namespace Symple
{
	class FunctionArgumentNode : public VariableDeclarationNode, public Function
	{
	private:
	public:
		FunctionArgumentNode(const TypeNode* type, const Token* name, const VariableModifiersNode* modifiers)
			: VariableDeclarationNode(name, type, modifiers, nullptr, nullptr) {}

		Kind GetKind() const override
		{
			return Kind::FunctionArgument;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Function Argument (" << mType->GetType()->GetName() << ") " << mName->GetLex();

			return ss.str();
		}
	};
}