#include "SympleCode/Binding/Binder.h"

namespace Symple::Binding
{
	shared_ptr<BoundExpression> Binder::BindExpressionInternal(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{
		switch (syntax->GetKind())
		{
		case Syntax::Node::BinaryExpression:
			return BindBinaryExpression(syntax->As<Syntax::BinaryExpressionSyntax>());
		default:
			return make_shared<BoundExpression>(syntax);
		}
	}

	shared_ptr<BoundExpression> Binder::BindExpression(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> result = BindExpressionInternal(syntax);
		if (result->GetType()->Is(Type::Error))
			return make_shared<BoundErrorExpression>(syntax);
	}

	shared_ptr<BoundBinaryExpression> Binder::BindBinaryExpression(shared_ptr<Syntax::BinaryExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> left = BindExpression(syntax->GetLeft());
		shared_ptr<BoundExpression> right = BindExpression(syntax->GetRight());

		shared_ptr<BoundBinaryOperator> op;

		return make_shared<BoundBinaryExpression>(syntax, op, left, right);
	}
}