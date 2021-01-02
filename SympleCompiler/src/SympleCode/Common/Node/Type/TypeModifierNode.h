#pragma once

#include "SympleCode/Common/Node/Type/TypeNodes.h"
#include "SympleCode/Common/Node/Node.h"
#include "SympleCode/Common/Token.h"

namespace Symple
{
	class TypeModifierNode : public Node, public TypeNodes
	{
	private:
		const Token* mModifier;
		bool mMutable;
	public:
		TypeModifierNode(const Token* modifier)
			: mModifier(modifier), mMutable(mModifier->Is(Token::Kind::Mutable))
		{}

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
			ss << "Type Modifier [" << mModifier->GetLex() << "]";

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