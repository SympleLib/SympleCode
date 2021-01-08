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

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Type Continue (" << mType->GetLex() << ")";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mModifiers->ToString(indent + newIndent, !mContinue);
			if (mContinue)
				ss << '\n' << mContinue->ToString(indent + newIndent);
			return ss.str();
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
		
		bool HasContinue(Token::Kind kind) const
		{
			if (mType->Is(kind))
				return true;
			if (mContinue)
				return mContinue->HasContinue(kind);
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
			bool type = mType->Is(other->mType->GetKind());
			bool hasContinue = mContinue && other->mContinue;
			bool contjnue = hasContinue && (mContinue->CanImplicitlyCastTo(other->mContinue) || mContinue == other->mContinue);
			bool modifiers = mModifiers->IsMutable() || !(mModifiers->IsMutable() || other->mModifiers->IsMutable());

			return type && ((modifiers && !hasContinue) || contjnue);
		}

		bool CanCastTo(const TypeContinueNode* other) const
		{
			bool type = mType->Is(other->mType->GetKind());
			bool modifiers = mModifiers->IsMutable() || !(mModifiers->IsMutable() || other->mModifiers->IsMutable());
			bool contjnue = mContinue == other->mContinue || (mContinue && other->mContinue && mContinue->CanCastTo(other->mContinue));

			return type && (modifiers || contjnue);
		}
	};
}