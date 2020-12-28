#pragma once

#include "SympleCode/Common/Analysis/Type.h"
#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Node.h"
#include "SympleCode/Common/Node/Function/FunctionModifierNode.h"

namespace Symple
{
	class FunctionModifiersNode : public Node, public Function
	{
	private:
		const std::vector<const FunctionModifierNode*> mModifiers;
		const FunctionModifierNode* mFormatType;
		bool mStatic;
	public:
		FunctionModifiersNode(const std::vector<const FunctionModifierNode*>& modifiers)
			: mModifiers(modifiers), mFormatType(), mStatic()
		{
			for (const FunctionModifierNode* modifier : mModifiers)
			{
				mStatic |= modifier->IsStatic();

				if (modifier->IsFormat())
					mFormatType = modifier;
			}
		}

		Kind GetKind() const override
		{
			return Kind::FunctionModifiers;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Function Modifiers";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			for (const FunctionModifierNode* modifier : mModifiers)
				ss << '\n' << modifier->ToString(indent + newIndent, modifier == mModifiers.back());

			return ss.str();
		}

		const std::vector<const FunctionModifierNode*>& GetModifiers() const
		{
			return mModifiers;
		}

		const FunctionModifierNode* GetFormatType() const
		{
			return mFormatType;
		}

		bool IsStatic() const
		{
			return mStatic;
		}
	};
}