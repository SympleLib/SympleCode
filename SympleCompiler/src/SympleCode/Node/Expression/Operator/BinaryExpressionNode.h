#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Expression/Operator/OperatorExpressionNode.h"

namespace Symple
{
	class BinaryExpressionNode : public OperatorExpressionNode
	{
	private:
		const ExpressionNode* mLeft;
		const ExpressionNode* mRight;

		int mEvaluate;
		bool mCanEvaluate;
	public:
		BinaryExpressionNode(const Token* oqerator, const ExpressionNode* left, const ExpressionNode* right)
			: OperatorExpressionNode(left->GetType(), oqerator), mLeft(left), mRight(right),
				mEvaluate(), mCanEvaluate(mLeft->CanEvaluate() && mRight->CanEvaluate())
		{
			if (!mRight->GetType()->CanImplicitlyCastTo(mLeft->GetType()))
			{
				Diagnostics::ReportError(mOperator, "Unmatched Types:\n%s,\n%s",
					mLeft->GetType()->ToString("", false).c_str(), mRight->GetType()->ToString().c_str());
				Diagnostics::ReportError(mOperator, "Left: %s, Right: %s",
					KindString(mLeft->GetKind()), KindString(mRight->GetKind()));
			}

			switch (mOperator->GetKind())
			{
			case Token::Kind::Pipe:
				mEvaluate = mLeft->Evaluate() | mRight->Evaluate();
				break;
			case Token::Kind::Ampersand:
				mEvaluate = mLeft->Evaluate() & mRight->Evaluate();
				break;

			case Token::Kind::EqualEqual: mType = BoolType;
				mEvaluate = mLeft->Evaluate() == mRight->Evaluate();
				break;
			case Token::Kind::ExclamationEqual: mType = BoolType;
				mEvaluate = mLeft->Evaluate() != mRight->Evaluate();
				break;
			case Token::Kind::PipePipe: mType = BoolType;
				mEvaluate = mLeft->Evaluate() || mRight->Evaluate();
				break;
			case Token::Kind::AmpersandAmpersand: mType = BoolType;
				mEvaluate = mLeft->Evaluate() && mRight->Evaluate();
				break;
			case Token::Kind::LeftArrow: mType = BoolType;
				mEvaluate = mLeft->Evaluate() < mRight->Evaluate();
				break;
			case Token::Kind::RightArrow: mType = BoolType;
				mEvaluate = mLeft->Evaluate() > mRight->Evaluate();
				break;
			case Token::Kind::LeftArrowEqual: mType = BoolType;
				mEvaluate = mLeft->Evaluate() <= mRight->Evaluate();
				break;
			case Token::Kind::RightArrowEqual: mType = BoolType;
				mEvaluate = mLeft->Evaluate() >= mRight->Evaluate();
				break;

			case Token::Kind::LeftArrowArrow:
				mEvaluate = mLeft->Evaluate() << mRight->Evaluate();
				break;
			case Token::Kind::RightArrowArrow:
				mEvaluate = mLeft->Evaluate() >> mRight->Evaluate();
				break;

			case Token::Kind::Plus:
				mEvaluate = mLeft->Evaluate() + mRight->Evaluate();
				break;
			case Token::Kind::Minus:
				mEvaluate = mLeft->Evaluate() - mRight->Evaluate();
				break;
			case Token::Kind::Asterisk:
				mEvaluate = mLeft->Evaluate() * mRight->Evaluate();
				break;
			case Token::Kind::Slash:
				if (mCanEvaluate &= (bool)mRight->Evaluate())
					mEvaluate = mLeft->Evaluate() / mRight->Evaluate();
				break;
			case Token::Kind::Percentage:
				if (mCanEvaluate &= (bool)mRight->Evaluate())
					mEvaluate = mLeft->Evaluate() % mRight->Evaluate();
				break;
			default:
				Diagnostics::ReportError(mOperator, "Invalid Operation!");
			}
		}

		Kind GetKind() const override
		{
			return Kind::BinaryExpression;
		}

		bool CanEvaluate() const override
		{
			return mCanEvaluate;
		}

		int Evaluate() const override
		{
			return mEvaluate;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Binary Expression [" << mOperator->GetLex() << "]";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mLeft->ToString(indent + newIndent, false);
			ss << '\n' << mRight->ToString(indent + newIndent);

			return ss.str();
		}
		
        const ExpressionNode* GetLeft() const { return mLeft; }

        const ExpressionNode* GetRight() const { return mRight; }
};
}