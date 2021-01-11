#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Expression/ExpressionNode.h"

#include "SympleCode/Analysis/Diagnostics.h"

namespace Symple
{
	class CastExpressionNode : public ExpressionNode
	{
	private:
		const Token* mOpen;
		const Token* mClose;
		const ExpressionNode* mExpression;
	public:
		CastExpressionNode(const Token* open, const TypeNode* type, const Token* close, const ExpressionNode* expression)
			: ExpressionNode(type), mOpen(open), mClose(close), mExpression(expression)
		{
			if (!mExpression->GetType()->CanCastTo(mType))
				Diagnostics::ReportError(open, "Illegal Cast");
		}

		Kind GetKind() const override
		{
			return Kind::CastExpression;
		}

		bool CanEvaluate() const override
		{
			return mExpression->CanEvaluate();
		}

		int Evaluate() const override
		{
			return mExpression->Evaluate();
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Cast Expression (" << mType->ToString() << ")";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mExpression->ToString(indent + newIndent);
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

		const ExpressionNode* GetExpression() const
		{
			return mExpression;
		}
	};
}