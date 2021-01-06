#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Node.h"
#include "SympleCode/Node/Type/TypeNodes.h"

#include "SympleCode/Analysis/Diagnostics.h"

namespace Symple
{
	class TypeModifierNode : public Node, public TypeNodes
	{
	private:
		bool mMutable, mUnsigned, mSigned;
		const Token* mModifier;
	public:
		static bool IsValid(Token::Kind kind)
		{
			switch (kind)
			{
			case Token::Kind::Mutable:
			case Token::Kind::Unsigned:
			case Token::Kind::Signed:
				return true;
			}

			return false;
		}

		TypeModifierNode(const Token* modifier)
			: mModifier(modifier), mMutable(modifier->Is(Token::Kind::Mutable)), mUnsigned(modifier->Is(Token::Kind::Unsigned)), mSigned(modifier->Is(Token::Kind::Signed))
		{
			if (!IsValid(mModifier->GetKind()))
				Diagnostics::ReportError(mModifier, "Illegal Modifier: %s", std::string(mModifier->GetLex()).c_str());
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

		bool IsUnsigned() const
		{
			return mUnsigned;
		}

		bool IsSigned() const
		{
			return mSigned;
		}
	};
}