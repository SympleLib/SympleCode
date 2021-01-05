#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Node/Type/TypeContinueNode.h"
#include "SympleCode/Common/Node/Type/TypeModifiersNode.h"

#include <iostream>

namespace Symple
{
	class TypeNode : public Node, public TypeNodes
	{
	private:
		const Type* mType;
		const TypeModifiersNode* mModifiers;
		const TypeContinueNode* mContinue;
	public:
		TypeNode(const Type* type, const TypeModifiersNode* modifiers, const TypeContinueNode* contjnue)
			: mType(type), mModifiers(modifiers), mContinue(contjnue)
		{}

		Kind GetKind() const
		{
			return Kind::Type;
		}

		const Type* GetType() const
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
			if (mContinue)
				return mContinue->HasContinue(type);
			return false;
		}

		bool SameAs(const TypeNode* other) const
		{
			bool rawType = mType == other->mType && (mContinue == other->mContinue || (mContinue && other->mContinue && mContinue->SameAs(other->mContinue)));
			bool modifiers = mModifiers->GetModifiers().size() == other->mModifiers->GetModifiers().size();
			if (modifiers)
				for (unsigned int i = 0; i < mModifiers->GetModifiers().size(); i++)
					modifiers &= mModifiers->GetModifiers()[i]->GetModifier()->Is(other->mModifiers->GetModifiers()[i]->GetModifier()->GetKind());

			return rawType && modifiers;
		}
	};
}