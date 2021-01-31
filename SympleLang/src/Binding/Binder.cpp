#include "SympleCode/Binding/Binder.h"

#include "SympleCode/Binding/Type.h"

namespace Symple::Binding
{
	shared_ptr<BoundExpression> Binder::BindExpressionInternal(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{
		switch (syntax->GetKind())
		{
		case Syntax::Node::UnaryExpression:
			return BindUnaryExpression(syntax->As<Syntax::UnaryExpressionSyntax>());
		case Syntax::Node::BinaryExpression:
			return BindBinaryExpression(syntax->As<Syntax::BinaryExpressionSyntax>());
		case Syntax::Node::LiteralExpression:
			return BindLiteralExpression(syntax->As<Syntax::LiteralExpressionSyntax>());
		default:
			return make_shared<BoundErrorExpression>(syntax);
		}
	}

	shared_ptr<BoundExpression> Binder::BindExpression(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> result = BindExpressionInternal(syntax);
		if (result->GetType()->Is(Type::Error))
			return make_shared<BoundErrorExpression>(syntax);
		return result;
	}

	shared_ptr<BoundUnaryExpression> Binder::BindUnaryExpression(shared_ptr<Syntax::UnaryExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> operand = BindExpression(syntax->GetOperand());

		shared_ptr<BoundUnaryOperator> op = BoundUnaryOperator::Bind(syntax->GetOperator()->GetKind(), operand->GetType());

		return make_shared<BoundUnaryExpression>(syntax, op, operand);
	}

	shared_ptr<BoundBinaryExpression> Binder::BindBinaryExpression(shared_ptr<Syntax::BinaryExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> left = BindExpression(syntax->GetLeft());
		shared_ptr<BoundExpression> right = BindExpression(syntax->GetRight());

		shared_ptr<BoundBinaryOperator> op = BoundBinaryOperator::Bind(syntax->GetOperator()->GetKind(), left->GetType(), right->GetType());

		return make_shared<BoundBinaryExpression>(syntax, op, left, right);
	}

	shared_ptr<BoundLiteralExpression> Binder::BindLiteralExpression(shared_ptr<Syntax::LiteralExpressionSyntax> syntax)
	{
		return make_shared<BoundLiteralExpression>(syntax, Type::IntType);
	}
}