#pragma once

#include "SympleCode/Common/Node/Type/TypeNodes.h"
#include "SympleCode/Common/Node/Type/TypeModifiersNode.h"
#include "SympleCode/Common/Type.h"

namespace Symple
{
	class TypeNode : public Node, public TypeNodes
	{
	private:
		const Type* mType;
		const TypeModifiersNode* mModifiers;
	public:
		TypeNode(const Type* type, const TypeModifiersNode* modifiers)
			: mType(type), mModifiers(modifiers)
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
	};
}