#pragma once

#include "SympleCode/Node/Member/FunctionDeclarationNode.h"

namespace Symple
{
	class ExternFunctionNode : public FunctionDeclarationNode
	{
	public:
		ExternFunctionNode(const TypeNode* type, const Token* name, const FunctionArgumentsNode* arguments, const FunctionModifiersNode* modifiers)
			: FunctionDeclarationNode(type, name, arguments, modifiers, nullptr) {}

		Kind GetKind() const override
		{
			return Kind::ExternFunction;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "External Function: " << mType->GetType()->GetName() << ' ' << mName->GetLex() << "()";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mArguments->ToString(indent + newIndent);
			ss << '\n' << mModifiers->ToString(indent + newIndent, false);

			return ss.str();
		}
	};
}