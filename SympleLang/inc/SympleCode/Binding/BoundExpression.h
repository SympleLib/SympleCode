#pragma once

#include "SympleCode/Binding/Node.h"
#include "SympleCode/Binding/Type.h"

namespace Symple::Binding
{
	class BoundExpression : public Node
	{
	public:
		BoundExpression(shared_ptr<Syntax::Node> syntax)
			: Node(syntax) {}

		virtual Kind GetKind()
		{ return Expression; }

		virtual shared_ptr<Type> GetType()
		{ return Type::ErrorType; }

		virtual bool IsMutable()
		{ return false; }
	};
}