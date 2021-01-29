#pragma once

#include "SympleCode/Binding/BoundExpression.h"

namespace Symple::Binding
{
	class BoundErrorExpression : public BoundExpression
	{
	public:
		virtual Kind GetKind() override
		{ return ErrorExpression; }

		virtual shared_ptr<Type> GetType() override
		{ return Type::ErrorType; }
	};
}