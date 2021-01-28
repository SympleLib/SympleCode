#pragma once

#include "SympleCode/Binding/BoundExpression.h"
#include "SympleCode/Binding/BoundBinaryOperator.h"

namespace Symple::Binding
{
	class BoundBinaryExpression : public BoundExpression
	{
	private:
		shared_ptr<BoundExpression> mLeft, mRight;
		shared_ptr<BoundBinaryOperator> mOperator;
	};
}