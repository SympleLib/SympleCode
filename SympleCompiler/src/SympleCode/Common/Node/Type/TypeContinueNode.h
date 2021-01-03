#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Node/Type/TypeModifiersNode.h"

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
	};
}