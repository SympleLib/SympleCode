#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Node/Type/TypeModifiersNode.h"

#include <iostream>

namespace Symple
{
	class TypeContinueNode : public Node, public TypeNodes
	{
	private:
		const Token* mType;
		const TypeModifiersNode* mModifiers;
		const TypeContinueNode* mContinue;
	public:
		TypeContinueNode(const Token* type, const TypeModifiersNode* modifiers, const TypeContinueNode* contjnue)
			: mType(type), mModifiers(modifiers), mContinue(contjnue)
		{
			if (!mType->IsEither({ Token::Kind::Asterisk }))
				Diagnostics::ReportError(mType, "Illegal Continue");
		}

		Kind GetKind() const
		{
			return Kind::TypeContinue;
		}

		const Token* GetType() const
		{
			return mType;
		}

		const TypeModifiersNode* GetModifiers() const
		{
			return mModifiers;
		}

		const TypeContinueNode* GetContinue() const
		{
			return mContinue;
		}
		
		bool HasContinue(const Token* type) const
		{
			if (mType == type)
				return true;
			if (mContinue)
				return mContinue->HasContinue(type);
			return false;
		}
		
		bool SameAs(const TypeContinueNode* other) const
		{
			bool rawType = mType->Is(other->mType->GetKind()) && (mContinue == other->mContinue || (mContinue && other->mContinue && mContinue->SameAs(other->mContinue)));
			bool modifiers = mModifiers->GetModifiers().size() == other->mModifiers->GetModifiers().size();
			if (modifiers)
				for (unsigned int i = 0; i < mModifiers->GetModifiers().size(); i++)
					modifiers &= mModifiers->GetModifiers()[i]->GetModifier()->Is(other->mModifiers->GetModifiers()[i]->GetModifier()->GetKind());

			return rawType && modifiers;
		}
	};
}