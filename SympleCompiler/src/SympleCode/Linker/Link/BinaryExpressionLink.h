#pragma once

#include "SympleCode/Linker/Link/ExpressionLink.h"

namespace Symple
{
	class BinaryExpressionLink : public ExpressionLink
	{
	private:
		const ExpressionLink* mLeft;
		const ExpressionLink* mRight;
	public:
		virtual Kind GetKind() const override
		{
			return Kind::BinaryExpression;
		}
	};
}