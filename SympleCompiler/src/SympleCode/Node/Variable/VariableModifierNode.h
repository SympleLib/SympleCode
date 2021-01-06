#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Node.h"
#include "SympleCode/Node/Variable/Variable.h"

#include "SympleCode/Analysis/Diagnostics.h"

namespace Symple
{
	class VariableModifierNode : public Node, public Variable
	{
	private:
		const Token* mModifier;
	public:
		static bool IsValid(Token::Kind kind)
		{
			return false;
		}

		VariableModifierNode(const Token* modifier)
			: mModifier(modifier)
		{
			if (!mModifier->IsEither({}))
				Diagnostics::ReportError(mModifier, "Illegal Modifier: %s", std::string(mModifier->GetLex()).c_str());
		}

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
	};
}