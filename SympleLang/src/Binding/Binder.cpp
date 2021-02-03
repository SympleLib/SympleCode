#include "SympleCode/Binding/Binder.h"

#include <string>

#include "SympleCode/Syntax/TypeReferenceSyntax.h"
#include "SympleCode/Syntax/ParenthesizedExpressionSyntax.h"

#include "SympleCode/Symbol/TypeSymbol.h"

#include "SympleCode/Binding/BoundImplicitCastExpression.h"

namespace Symple::Binding
{
	shared_ptr<BoundExpression> Binder::BindExpressionInternal(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{
		switch (syntax->GetKind())
		{
		case Syntax::Node::UnaryExpression:
			return BindUnaryExpression(dynamic_pointer_cast<Syntax::UnaryExpressionSyntax>(syntax));
		case Syntax::Node::BinaryExpression:
			return BindBinaryExpression(dynamic_pointer_cast<Syntax::BinaryExpressionSyntax>(syntax));
		case Syntax::Node::LiteralExpression:
			return BindLiteralExpression(dynamic_pointer_cast<Syntax::LiteralExpressionSyntax>(syntax));
		case Syntax::Node::ParenthesizedExpression:
			return BindExpressionInternal(dynamic_pointer_cast<Syntax::ParenthesizedExpressionSyntax>(syntax)->GetExpression());
		default:
			return make_shared<BoundErrorExpression>(syntax);
		}
	}

	shared_ptr<Node> Binder::Bind(shared_ptr<Syntax::Node> syntax)
	{
		if (dynamic_pointer_cast<Syntax::ExpressionSyntax, Syntax::Node>(syntax))
			return BindExpression(dynamic_pointer_cast<Syntax::ExpressionSyntax>(syntax));
		else
			return make_shared<Node>(syntax);
	}


#define TYPE_CONT(name) \
		case Syntax::Token::##name##Keyword: \
			return make_shared<Symbol::TypeSymbol>(Symbol::TypeSymbol::##name##Type->GetTypeKind(), Symbol::TypeSymbol::##name##Type->GetName(), Symbol::TypeSymbol::##name##Type->GetSize(), base)

#define TYPE_CASE(name) \
		case Syntax::Token::##name##Keyword: \
			return Symbol::TypeSymbol::##name##Type

	shared_ptr<Symbol::TypeSymbol> Binder::BindType(shared_ptr<Syntax::TypeSyntax> syntax)
	{
		if (dynamic_pointer_cast<Syntax::TypeReferenceSyntax>(syntax) && dynamic_pointer_cast<Syntax::TypeReferenceSyntax>(syntax)->GetBase())
		{
			shared_ptr<Symbol::TypeSymbol> base = BindType(dynamic_pointer_cast<Syntax::TypeReferenceSyntax>(syntax)->GetBase());

			switch (syntax->GetName()->GetKind())
			{
				TYPE_CONT(Void);

				TYPE_CONT(Byte);
				TYPE_CONT(Short);
				TYPE_CONT(Int);
				TYPE_CONT(Long);

				TYPE_CONT(Bool);
				TYPE_CONT(Char);
				TYPE_CONT(WChar);

				TYPE_CONT(Float);
				TYPE_CONT(Double);
				TYPE_CONT(Triple);

				// Special
			case Syntax::Token::Asterisk:
				return make_shared<Symbol::TypeSymbol>(Symbol::TypeSymbol::VoidPointerType->GetTypeKind(), Symbol::TypeSymbol::VoidPointerType->GetName(), Symbol::TypeSymbol::VoidPointerType->GetSize(), base);
			}
		}
		else
		{
			switch (syntax->GetName()->GetKind())
			{
				TYPE_CASE(Void);

				TYPE_CASE(Byte);
				TYPE_CASE(Short);
				TYPE_CASE(Int);
				TYPE_CASE(Long);

				TYPE_CASE(Bool);
				TYPE_CASE(Char);
				TYPE_CASE(WChar);

				TYPE_CASE(Float);
				TYPE_CASE(Double);
				TYPE_CASE(Triple);

				// Special
			case Syntax::Token::Asterisk:
				return Symbol::TypeSymbol::VoidPointerType;
			}
		}
	}


	shared_ptr<BoundExpression> Binder::BindExpression(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> result = BindExpressionInternal(syntax);
		if (!result /* Should not be null, but just in case */ || result->GetType()->Is(Symbol::TypeSymbol::Error))
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
		shared_ptr<Symbol::TypeSymbol> ty;
		switch (syntax->GetLiteral()->GetKind())
		{
		case Syntax::Token::Integer:
			if (std::stoll(std::string(syntax->GetLiteral()->GetText())) & 0xFFFFFFFF00000000)
				ty = Symbol::TypeSymbol::LongType;
			else
				ty = Symbol::TypeSymbol::IntType;
			break;
		case Syntax::Token::Number:
			ty = Symbol::TypeSymbol::DoubleType;
			break;
		case Syntax::Token::Float:
			ty = Symbol::TypeSymbol::FloatType;
			break;
		default:
			ty = Symbol::TypeSymbol::ErrorType;
			break;
		}

		return make_shared<BoundLiteralExpression>(syntax, ty);
	}

	shared_ptr<DiagnosticBag> Binder::GetDiagnosticBag()
	{ return mDiagnosticBag; }
}