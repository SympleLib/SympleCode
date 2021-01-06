#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Node/Type/TypeContinueNode.h"
#include "SympleCode/Node/Type/TypeModifiersNode.h"

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

		Kind GetKind() const override
		{
			return Kind::Type;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Type (" << mType->GetName() << ' ' << GetSize() << " bytes)";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mModifiers->ToString(indent + newIndent, !mContinue);
			if (mContinue)
				ss << '\n' << mContinue->ToString(indent + newIndent);
			return ss.str();
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

		unsigned int GetSize() const
		{
			return HasContinue(Token::Kind::Asterisk) ? 4 : mType->GetSize();
		}

		bool HasContinue(Token::Kind kind) const
		{
			if (mContinue)
				return mContinue->HasContinue(kind);
			return false;
		}

		bool SameAs(const TypeNode* other) const
		{
			bool rawType = mType == other->mType && (mContinue == other->mContinue || (mContinue && other->mContinue && mContinue->SameAs(other->mContinue)));
			bool modifiers = mModifiers->IsMutable() == other->mModifiers->IsMutable();

			return rawType && modifiers;
		}

		bool CanImplicitlyCastTo(const TypeNode* other) const
		{
			bool type = mType == other->mType;
			bool contjnue = mContinue == other->mContinue || (mContinue && other->mContinue && mContinue->CanImplicitlyCastTo(other->mContinue));
			bool modifiers = mModifiers->IsMutable() || !(mModifiers->IsMutable() || other->mModifiers->IsMutable());

			return type && (modifiers || contjnue);
		}

		bool CanCastTo(const TypeNode* other) const
		{
			bool type = GetSize() == other->GetSize() || mType == other->mType;
			bool modifiers = mModifiers->IsMutable() || !(mModifiers->IsMutable() || other->mModifiers->IsMutable());
			bool contjnue = mContinue == other->mContinue || (mContinue && other->mContinue && mContinue->CanCastTo(other->mContinue));

			return type && (modifiers || contjnue);
		}
	};

	extern const TypeModifierNode* const MutModifier;
	extern const TypeModifiersNode* const MutModifiers;
	extern const TypeModifiersNode* const EmptyModifiers;

	extern const Token* const PtrToken;
	extern const TypeContinueNode* const PtrContinue;

	extern const TypeNode* const VoidType;
	extern const TypeNode* const ByteType;
	extern const TypeNode* const ShortType;
	extern const TypeNode* const IntType;

	extern const TypeNode* const BoolType;
	extern const TypeNode* const CharType;
	extern const TypeNode* const WCharType;

	extern const TypeNode* const PtrType;
	extern const TypeNode* const StringType;

	extern const TypeNode* const ErrorType;
}