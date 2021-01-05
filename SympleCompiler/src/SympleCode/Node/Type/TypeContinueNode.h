#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Node/Type/TypeModifiersNode.h"

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
			bool modifiers = mModifiers->IsMutable() == other->mModifiers->IsMutable();

			return rawType && modifiers;
		}

		bool CanImplicitlyCastTo(const TypeContinueNode* other) const
		{
			bool rawType = mType->Is(other->mType->GetKind()) && (mContinue == other->mContinue || (mContinue && other->mContinue && mContinue->CanImplicitlyCastTo(other->mContinue)));
			bool modifiers = mModifiers->IsMutable() || !(mModifiers->IsMutable() || other->mModifiers->IsMutable());

			return rawType && modifiers;
		}
	};
}