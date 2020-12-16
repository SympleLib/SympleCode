#pragma once

#include "SympleCode/Common/Node/Member/FunctionDeclarationNode.h"

namespace Symple
{
	class FunctionHintNode : public FunctionDeclarationNode
	{
	public:
		FunctionHintNode(const Type* type, const Token* name, const FunctionArgumentsNode* arguments)
			: FunctionDeclarationNode(type, name, arguments, nullptr) {}

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
			ss << "Function Hint: " << mType->GetName() << ' ' << mName->GetLex() << "()";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mArguments->ToString(indent + newIndent);

			return ss.str();
		}
	};
}