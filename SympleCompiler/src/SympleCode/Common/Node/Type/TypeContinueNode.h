#pragma once

#include "SympleCode/Common/Node/Type/TypeModifierNode.h"

namespace Symple
{
	class TypeContinueNode : public TypeModifierNode
	{
	public:
		TypeContinueNode(const Token* contjnue)
			
		{
			if (!mModifier->IsEither({ Token::Kind::Asterisk }))
				Diagnostics::ReportError(mModifier, "Illegal Modifier");
		}

		Kind GetKind() const override
		{
			return Kind::TypeModifier;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Type Continue [" << mModifier->GetLex() << "]";

			return ss.str();
		}

		const Token* GetModifier() const
		{
			return mModifier;
		}
	};
}