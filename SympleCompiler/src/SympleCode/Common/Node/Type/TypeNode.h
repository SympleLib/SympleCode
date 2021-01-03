#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Node/Type/TypeContinueNode.h"
#include "SympleCode/Common/Node/Type/TypeModifiersNode.h"

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
	};
}