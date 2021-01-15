#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Analysis/Debug.h"

namespace Symple
{
	class StallocExpressionNode : public ExpressionNode, public Function
	{
	private:
		const Token* mOpen;
		const ExpressionNode* mSize;
		const Token* mClose;
	public:
		StallocExpressionNode(const Token* open, const ExpressionNode* size, const Token* close)
			: ExpressionNode(PtrType), mOpen(open), mSize(size), mClose(close)
		{}

		Kind GetKind() const
		{
			return Kind::StallocExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Stalloc Expression";
			if (mSize->CanEvaluate())
				ss << ' ' << mSize->Evaluate();

			return ss.str();
		}

		const Token* GetOpen() const
		{
			return mOpen;
		}

		const Token* GetClose() const
		{
			return mClose;
		}

		const ExpressionNode* GetSize() const
		{
			return mSize;
		}
	};
}