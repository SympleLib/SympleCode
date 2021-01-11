#pragma once

#include "SympleCode/Node/Member/GlobalVariableDeclarationNode.h"

namespace Symple
{
	class SharedVariableNode : public GlobalVariableDeclarationNode
	{
	public:
		SharedVariableNode(const Token* name, const TypeNode* type, const VariableModifiersNode* modifiers, const SharedVariableNode* next)
			: GlobalVariableDeclarationNode(name, type, modifiers, nullptr, next) {}

		Kind GetKind() const override
		{
			return Kind::SharedVariable;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Shared Variable (" << mType->GetType()->GetName() << ") " << mName->GetLex();
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";

			ss << '\n' << mModifiers->ToString(indent + newIndent, !mNext);
			if (mNext)
				ss << '\n' << mNext->ToString(indent + newIndent);

			return ss.str();
		}
	};
}