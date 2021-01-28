#pragma once

#include "SympleCode/Binding/BoundExpression.h"

namespace Symple::Binding
{
	class BoundBinaryExpression : public BoundExpression
	{
	private:
		shared_ptr<BoundExpression> mLeft, mRight;
	};
}