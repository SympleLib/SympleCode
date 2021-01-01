#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Node.h"
#include "SympleCode/Common/Node/Variable/Variable.h"

namespace Symple
{
	class VariableModifierNode : public Node, public Variable
	{
	private:
		const Token* mModifier;
		bool mMutable;
	public:
		VariableModifierNode(const Token* modifier)
			: mModifier(modifier), mMutable(mModifier->Is(Token::Kind::Mutable))
		{}

		Kind GetKind() const override
		{
			return Kind::VariableModifier;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Variable Modifier [" << mModifier->GetLex() << "]";

			return ss.str();
		}

		const Token* GetModifier() const
		{
			return mModifier;
		}

		bool IsMutable() const
		{
			return mMutable;
		}
	};
}