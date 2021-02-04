#include "SympleCode/Binding/Binder.h"

#include <string>

#include "SympleCode/Syntax/TypeReferenceSyntax.h"
#include "SympleCode/Syntax/ParenthesizedExpressionSyntax.h"

#include "SympleCode/Symbol/TypeSymbol.h"

#include "SympleCode/Binding/BoundImplicitCastExpression.h"

namespace Symple::Binding
{
	shared_ptr<BoundCompilationUnit> Binder::Bind(shared_ptr<Syntax::TranslationUnitSyntax> unit)
	{
		mCompilationUnit = unit;
		mFunctions.clear();

		for (auto member : mCompilationUnit->GetMembers())
			BindMember(member);

		return make_shared<BoundCompilationUnit>(unit, mFunctions);
	}


	#pragma region Symbols

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

			default:
				return Symbol::TypeSymbol::ErrorType;
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

			default:
				return Symbol::TypeSymbol::ErrorType;
			}
		}
	}

	shared_ptr<Symbol::FunctionSymbol> Binder::BindFunction(shared_ptr<Syntax::FunctionDeclarationSyntax> syntax)
	{
		shared_ptr<Symbol::TypeSymbol> ty = BindType(syntax->GetType());
		std::string_view name = syntax->GetName()->GetText();
		Symbol::ParameterList params;
		for (auto param : syntax->GetParameters())
			params.push_back(BindParameter(param));

		shared_ptr<Symbol::FunctionSymbol> symbol = make_shared<Symbol::FunctionSymbol>(ty, name, params);
		shared_ptr<BoundStatement> body = BindStatement(syntax->GetBody());
		mFunctions.insert({ symbol, body });
		return symbol;
	}

	shared_ptr<Symbol::ParameterSymbol> Binder::BindParameter(shared_ptr<Syntax::VariableDeclarationSyntax> syntax)
	{
		shared_ptr<Symbol::TypeSymbol> ty = BindType(syntax->GetType());
		std::string_view name = syntax->GetName()->GetText();
		shared_ptr<BoundConstant> init;
		if (syntax->GetInitializer())
			init = BindExpression(syntax->GetInitializer())->ConstantValue();

		return make_shared<Symbol::ParameterSymbol>(ty, name, init);
	}

	#pragma endregion


	#pragma region Members

	shared_ptr<Node> Binder::BindMember(shared_ptr<Syntax::MemberSyntax> syntax)
	{
		shared_ptr<Node> result = BindMemberInternal(syntax);
		if (!result /* Should not be null, but just in case */)
		{
			mDiagnosticBag->ReportUnimplimentedError(syntax->GetToken());
			return make_shared<Node>(syntax);
		}
		else
			return result;
	}

	shared_ptr<Node> Binder::BindMemberInternal(shared_ptr<Syntax::MemberSyntax> syntax)
	{
		switch (syntax->GetKind())
		{
		case Syntax::Node::GlobalStatement:
			return BindGlobalStatement(dynamic_pointer_cast<Syntax::GlobalStatementSyntax>(syntax));
		case Syntax::Node::FunctionDeclaration:
			BindFunction(dynamic_pointer_cast<Syntax::FunctionDeclarationSyntax>(syntax));
		default:
			return make_shared<Node>(syntax);
		}
	}

	shared_ptr<BoundStatement> Binder::BindGlobalStatement(shared_ptr<Syntax::GlobalStatementSyntax> syntax)
	{ return BindStatement(syntax->GetStatement()); }

	#pragma endregion


	#pragma region Statement

	shared_ptr<BoundStatement> Binder::BindStatement(shared_ptr<Syntax::StatementSyntax> syntax)
	{
		shared_ptr<BoundStatement> result = BindStatementInternal(syntax);
		if (!result /* Should not be null, but just in case */)
		{
			mDiagnosticBag->ReportUnimplimentedError(syntax->GetToken());
			return make_shared<BoundStatement>(syntax);
		}
		else
			return result;
	}

	shared_ptr<BoundStatement> Binder::BindStatementInternal(shared_ptr<Syntax::StatementSyntax> syntax)
	{
		switch (syntax->GetKind())
		{
		case Syntax::Node::BlockStatement:
			return BindBlockStatement(dynamic_pointer_cast<Syntax::BlockStatementSyntax>(syntax));
		case Syntax::Node::ReturnStatement:
			return BindReturnStatement(dynamic_pointer_cast<Syntax::ReturnStatementSyntax>(syntax));
		default:
			return make_shared<BoundStatement>(syntax);
		}
	}

	shared_ptr<BoundBlockStatement> Binder::BindBlockStatement(shared_ptr<Syntax::BlockStatementSyntax> syntax)
	{
		std::vector<shared_ptr<BoundStatement>> statements;
		for (auto statement : syntax->GetStatements())
			statements.push_back(BindStatement(statement));

		return make_shared<BoundBlockStatement>(syntax, statements);
	}

	shared_ptr<BoundReturnStatement> Binder::BindReturnStatement(shared_ptr<Syntax::ReturnStatementSyntax> syntax)
	{ return make_shared<BoundReturnStatement>(syntax, BindExpression(syntax->GetValue())); }

	#pragma endregion


	#pragma region Expression

	shared_ptr<BoundExpression> Binder::BindExpression(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> result = BindExpressionInternal(syntax);
		if (!result /* Should not be null, but just in case */ || result->GetType()->Is(Symbol::TypeSymbol::Error))
		{
			mDiagnosticBag->ReportUnimplimentedError(syntax->GetToken());
			return make_shared<BoundErrorExpression>(syntax);
		}
		else
			return result;
	}

	shared_ptr<BoundExpression> Binder::BindExpressionInternal(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{
		switch (syntax->GetKind())
		{
		case Syntax::Node::CallExpression:
			return BindCallExpression(dynamic_pointer_cast<Syntax::CallExpressionSyntax>(syntax));
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

	shared_ptr<BoundCallExpression> Binder::BindCallExpression(shared_ptr<Syntax::CallExpressionSyntax> syntax)
	{
		shared_ptr<Symbol::FunctionSymbol> funcSymbol;
		for (auto& func : mFunctions)
			if (func.first->GetName() == syntax->GetName()->GetText())
				funcSymbol = func.first;

		ExpressionList args;
		for (auto arg : syntax->GetArguments())
			args.push_back(BindExpression(arg));

		return make_shared<BoundCallExpression>(syntax, funcSymbol, args);
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

	#pragma endregion


	shared_ptr<DiagnosticBag> Binder::GetDiagnosticBag()
	{ return mDiagnosticBag; }
}