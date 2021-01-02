#pragma once

#include "SympleCode/Common/Node/Member/FunctionDeclarationNode.h"

namespace Symple
{
	class FunctionHintNode : public FunctionDeclarationNode
	{
	public:
		FunctionHintNode(const TypeNode* type, const Token* name, const FunctionArgumentsNode* arguments, const FunctionModifiersNode* modifiers)
			: FunctionDeclarationNode(type, name, arguments, modifiers, nullptr) {}

		Kind GetKind() const override
		{
			return Kind::FunctionHint;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Function Hint: " << mType->GetType()->GetName() << ' ' << mName->GetLex() << "()";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mArguments->ToString(indent + newIndent);
			ss << '\n' << mModifiers->ToString(indent + newIndent, false);

			return ss.str();
		}
	};
}