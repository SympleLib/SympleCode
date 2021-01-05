#pragma once

#include "SympleCode/Node/Type/TypeNode.h"
#include "SympleCode/Node/Node.h"

namespace Symple
{
	class ExpressionNode : public Node
	{
	protected:
		const TypeNode* mType;
	public:
		ExpressionNode(const TypeNode* type)
			: mType(type) {}

		virtual Kind GetKind() const override
		{
			return Kind::Expression;
		}

		const TypeNode* GetType() const
		{
			return mType;
		}
	};
}