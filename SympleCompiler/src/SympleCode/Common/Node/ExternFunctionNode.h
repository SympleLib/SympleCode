#pragma once

#include "SympleCode/Common/Node/FunctionDeclarationNode.h"

namespace Symple
{
	class ExternFunctionNode : public FunctionDeclarationNode
	{
	public:
		ExternFunctionNode(const Type* type, const Token* name, const FunctionArgumentsNode* arguments)
			: FunctionDeclarationNode(type, name, arguments, nullptr) {}

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
			ss << "External Function: " << mType->GetName() << ' ' << mName->GetLex() << "()";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mArguments->ToString(indent + newIndent);

			return ss.str();
		}
	};
}