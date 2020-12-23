#pragma once

#include "SympleCode/Common/Analysis/Type.h"
#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Statement/VariableDeclarationNode.h"

namespace Symple
{
	class FunctionArgumentNode : public VariableDeclarationNode
	{
	private:
	public:
		FunctionArgumentNode(const Type* type, const Token* name)
			: VariableDeclarationNode(name, type, nullptr) {}

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
			ss << "Function Argument (" << mType->GetName() << ") " << mName->GetLex();

			return ss.str();
		}
	};
}