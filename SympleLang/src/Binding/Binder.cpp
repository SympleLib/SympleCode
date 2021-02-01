#include "SympleCode/Binding/Binder.h"

#include <string>

#include "SympleCode/Syntax/ParenthesizedExpressionSyntax.h"

#include "SympleCode/Binding/Type.h"
#include "SympleCode/Binding/BoundImplicitCastExpression.h"

namespace Symple::Binding
{
	shared_ptr<BoundExpression> Binder::BindExpressionInternal(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{
		switch (syntax->GetKind())
		{
		case Syntax::Node::UnaryExpression:
			return BindUnaryExpression(dynamic_pointer_cast<Syntax::UnaryExpressionSyntax, Syntax::ExpressionSyntax>(syntax));
		case Syntax::Node::BinaryExpression:
			return BindBinaryExpression(dynamic_pointer_cast<Syntax::BinaryExpressionSyntax, Syntax::ExpressionSyntax>(syntax));
		case Syntax::Node::LiteralExpression:
			return BindLiteralExpression(dynamic_pointer_cast<Syntax::LiteralExpressionSyntax, Syntax::ExpressionSyntax>(syntax));
		case Syntax::Node::ParenthesizedExpression:
			return BindExpressionInternal(dynamic_pointer_cast<Syntax::ParenthesizedExpressionSyntax, Syntax::ExpressionSyntax>(syntax)->GetExpression());
		default:
			return make_shared<BoundErrorExpression>(syntax);
		}
	}

	shared_ptr<BoundExpression> Binder::BindExpression(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> result = BindExpressionInternal(syntax);
		if (!result /* Should not be null, but just in case */ || result->GetType()->Is(Type::Error))
			return make_shared<BoundErrorExpression>(syntax);
		return result;
	}

	shared_ptr<BoundUnaryExpression> Binder::BindUnaryExpression(shared_ptr<Syntax::UnaryExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> operand = BindExpression(syntax->GetOperand());

		shared_ptr<BoundUnaryOperator> op = BoundUnaryOperator::Bind(syntax->GetOperator()->GetKind(), operand->GetType());
		if (op == BoundUnaryOperator::ErrorOperator)
			mDiagnosticBag->ReportInvalidOperation(syntax->GetOperator(), operand->GetType());

		return make_shared<BoundUnaryExpression>(syntax, op, operand);
	}

	shared_ptr<BoundBinaryExpression> Binder::BindBinaryExpression(shared_ptr<Syntax::BinaryExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> left = BindExpression(syntax->GetLeft());
		shared_ptr<BoundExpression> right = BindExpression(syntax->GetRight());

		shared_ptr<BoundBinaryOperator> op = BoundBinaryOperator::Bind(syntax->GetOperator()->GetKind(), left->GetType(), right->GetType());
		if (op == BoundBinaryOperator::ErrorOperator)
			mDiagnosticBag->ReportInvalidOperation(syntax->GetOperator(), left->GetType(), right->GetType());
		else
		{
			if (left->GetType()->Equals(op->GetLeftType()))
				left = make_shared<BoundImplicitCastExpression>(syntax->GetLeft(), op->GetLeftType(), left);
			if (right->GetType()->Equals(op->GetRightType()))
				right = make_shared<BoundImplicitCastExpression>(syntax->GetRight(), op->GetRightType(), right);
		}

		return make_shared<BoundBinaryExpression>(syntax, op, left, right);
	}

	shared_ptr<BoundLiteralExpression> Binder::BindLiteralExpression(shared_ptr<Syntax::LiteralExpressionSyntax> syntax)
	{
		shared_ptr<Type> ty;
		switch (syntax->GetLiteral()->GetKind())
		{
		case Syntax::Token::Integer:
			if (std::stoll(std::string(syntax->GetLiteral()->GetText())) & 0xFFFFFFFF00000000)
				ty = Type::LongType;
			else
				ty = Type::IntType;
			break;
		case Syntax::Token::Number:
			ty = Type::DoubleType;
			break;
		case Syntax::Token::Float:
			ty = Type::FloatType;
			break;
		default:
			ty = Type::ErrorType;
			break;
		}

		return make_shared<BoundLiteralExpression>(syntax, ty);
	}

	shared_ptr<DiagnosticBag> Binder::GetDiagnosticBag()
	{ return mDiagnosticBag; }
}