#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Node.h"
#include "SympleCode/Node/Variable/VariableModifierNode.h"

namespace Symple
{
	class VariableModifiersNode : public Node, public Variable
	{
	private:
		const std::vector<const VariableModifierNode*> mModifiers;

		bool mPrivate;
	public:
		VariableModifiersNode(const std::vector<const VariableModifierNode*>& modifiers)
			: mModifiers(modifiers), mPrivate()
		{
			for (const VariableModifierNode* modifier : mModifiers)
			{
				mPrivate |= modifier->IsPrivate();
			}
		}

		Kind GetKind() const override
		{
			return Kind::VariableModifiers;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Variable Modifiers";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			for (const VariableModifierNode* modifier : mModifiers)
				ss << '\n' << modifier->ToString(indent + newIndent, modifier == mModifiers.back());

			return ss.str();
		}

		const std::vector<const VariableModifierNode*>& GetModifiers() const
		{
			return mModifiers;
		}

		bool IsPrivate() const
		{
			return mPrivate;
		}
	};
}