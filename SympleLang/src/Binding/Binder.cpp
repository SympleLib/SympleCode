#include "SympleCode/Binding/Binder.h"

#include <string>

#include "SympleCode/Syntax/TypeReferenceSyntax.h"
#include "SympleCode/Syntax/ParenthesizedExpressionSyntax.h"

#include "SympleCode/Symbol/TypeSymbol.h"

#include "SympleCode/Binding/BoundDefaultExpression.h"
#include "SympleCode/Binding/BoundConstantExpression.h"
#include "SympleCode/Binding/BoundImplicitCastExpression.h"

namespace Symple::Binding
{
	void Binder::BeginScope()
	{ mScope = make_shared<BoundScope>(mScope); }

	void Binder::EndScope()
	{ mScope = mScope->GetBase(); }

	shared_ptr<BoundCompilationUnit> Binder::Bind(shared_ptr<Syntax::TranslationUnitSyntax> unit)
	{
		mCompilationUnit = unit;
		mFunctions.clear();
		mScope.reset();
		BeginScope();

		for (auto member : mCompilationUnit->GetMembers())
			BindMember(member);

		EndScope();

		return make_shared<BoundCompilationUnit>(unit, mFunctions);
	}


	#pragma region Symbols

#define TYPE_CONT(name) \
		case Syntax::Token::##name##Keyword: \
			return make_shared<Symbol::TypeSymbol>(Symbol::TypeSymbol::##name##Type->GetTypeKind(), Symbol::TypeSymbol::##name##Type->GetName(), Symbol::TypeSymbol::##name##Type->GetSize(), Symbol::TypeSymbol::##name##Type->IsFloat(), base)

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
				return make_shared<Symbol::TypeSymbol>(Symbol::TypeSymbol::VoidPointerType->GetTypeKind(), Symbol::TypeSymbol::VoidPointerType->GetName(), Symbol::TypeSymbol::VoidPointerType->GetSize(), false, base);

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

		BeginScope();
		for (auto param : symbol->GetParameters())
			mScope->DeclareVariable(param);
		shared_ptr<BoundStatement> body = BindStatement(syntax->GetBody());
		EndScope();

		mFunctions.push_back({ symbol, body });
		return symbol;
	}

	shared_ptr<Symbol::FunctionSymbol> Binder::BindExternFunction(shared_ptr<Syntax::ExternFunctionSyntax> syntax)
	{
		shared_ptr<Symbol::TypeSymbol> ty = BindType(syntax->GetType());
		std::string_view name = syntax->GetName()->GetText();
		Symbol::ParameterList params;
		for (auto param : syntax->GetParameters())
			params.push_back(BindParameter(param));

		shared_ptr<Symbol::FunctionSymbol> symbol = make_shared<Symbol::FunctionSymbol>(ty, name, params);

		mFunctions.push_back({ symbol, nullptr });
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
			mDiagnosticBag->ReportBindError(syntax);
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
		case Syntax::Node::ExternFunction:
			BindExternFunction(dynamic_pointer_cast<Syntax::ExternFunctionSyntax>(syntax));
			goto Return;
		case Syntax::Node::FunctionDeclaration:
			BindFunction(dynamic_pointer_cast<Syntax::FunctionDeclarationSyntax>(syntax));
			goto Return;
		default:
		Return:
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
			mDiagnosticBag->ReportBindError(syntax);
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
		case Syntax::Node::ExpressionStatement:
			return BindExpressionStatement(dynamic_pointer_cast<Syntax::ExpressionStatementSyntax>(syntax));
		default:
			return make_shared<BoundStatement>(syntax);
		}
	}

	shared_ptr<BoundBlockStatement> Binder::BindBlockStatement(shared_ptr<Syntax::BlockStatementSyntax> syntax)
	{
		std::vector<shared_ptr<BoundStatement>> statements;

		BeginScope();
		for (auto statement : syntax->GetStatements())
			statements.push_back(BindStatement(statement));
		EndScope();

		return make_shared<BoundBlockStatement>(syntax, statements);
	}

	shared_ptr<BoundReturnStatement> Binder::BindReturnStatement(shared_ptr<Syntax::ReturnStatementSyntax> syntax)
	{ return make_shared<BoundReturnStatement>(syntax, BindExpression(syntax->GetValue())); }

	shared_ptr<BoundExpressionStatement> Binder::BindExpressionStatement(shared_ptr<Syntax::ExpressionStatementSyntax> syntax)
	{ return make_shared<BoundExpressionStatement>(syntax, BindExpression(syntax->GetExpression())); }

	#pragma endregion


	#pragma region Expression

	shared_ptr<BoundExpression> Binder::BindExpression(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> result = BindExpressionInternal(syntax);
		if (!result /* Should not be null, but just in case */ || result->GetType()->Is(Symbol::TypeSymbol::Error))
		{
			mDiagnosticBag->ReportExpressionMustHaveValue(syntax);
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
		case Syntax::Node::NameExpression:
			return BindNameExpression(dynamic_pointer_cast<Syntax::NameExpressionSyntax>(syntax));
		case Syntax::Node::ParenthesizedExpression:
			return BindExpressionInternal(dynamic_pointer_cast<Syntax::ParenthesizedExpressionSyntax>(syntax)->GetExpression());
		default:
			return make_shared<BoundErrorExpression>(syntax);
		}
	}

	shared_ptr<BoundExpression> Binder::BindCallExpression(shared_ptr<Syntax::CallExpressionSyntax> syntax)
	{
		shared_ptr<Symbol::FunctionSymbol> funcSymbol = FindFunction(mFunctions, syntax->GetName()->GetText());
		if (!funcSymbol)
		{
			mDiagnosticBag->ReportNoSuchFunction(syntax);
			return make_shared<BoundErrorExpression>(syntax);
		}

		ExpressionList args;
		if (syntax->GetArguments().size() > funcSymbol->GetParameters().size())
			mDiagnosticBag->ReportTooManyArguments(syntax, funcSymbol->GetParameters().size());
		else
			for (unsigned i = 0; i < funcSymbol->GetParameters().size(); i++)
			{
				shared_ptr<BoundExpression> arg = make_shared<BoundConstantExpression>(funcSymbol->GetParameters()[i]->GetInitializer());
				if (i < syntax->GetArguments().size())
				{
					auto boundArg = BindExpression(syntax->GetArguments()[i]);
					if (boundArg->GetKind() == Node::DefaultExpression)
					{
						if (!arg->ConstantValue())
						{
							mDiagnosticBag->ReportNoDefaultArgument(syntax, i);
							break;
						}
					}
					else
						arg = boundArg;
				}
				else if (!arg->ConstantValue())
				{
					mDiagnosticBag->ReportTooFewArguments(syntax);
					break;
				}
				
				args.push_back(arg);
			}

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

		return make_shared<BoundBinaryExpression>(syntax, op, left, right);
	}

	shared_ptr<BoundExpression> Binder::BindLiteralExpression(shared_ptr<Syntax::LiteralExpressionSyntax> syntax)
	{
		shared_ptr<Symbol::TypeSymbol> ty;
		shared_ptr<BoundConstant> constant;
		switch (syntax->GetLiteral()->GetKind())
		{
		case Syntax::Token::Integer:
		{
			if (std::stoll(std::string(syntax->GetLiteral()->GetText())) & 0xFFFFFFFF00000000)
				ty = Symbol::TypeSymbol::LongType;
			else
				ty = Symbol::TypeSymbol::IntType;

			long long val = std::stoll(std::string(syntax->GetLiteral()->GetText()));
			constant = make_shared<BoundConstant>(BoundConstant::Integer, &val);
			break;
		}
		case Syntax::Token::Number:
		{
			ty = Symbol::TypeSymbol::DoubleType;

			double val = std::stod(std::string(syntax->GetLiteral()->GetText()));
			constant = make_shared<BoundConstant>(BoundConstant::Float, &val);
			break;
		}
		case Syntax::Token::Float:
		{
			ty = Symbol::TypeSymbol::FloatType;

			float val = std::stof(std::string(syntax->GetLiteral()->GetText()));
			constant = make_shared<BoundConstant>(BoundConstant::Float, &val);
			break;
		}
		case Syntax::Token::DefaultKeyword:
		{
			return make_shared<BoundDefaultExpression>(syntax);
		}
		default:
			ty = Symbol::TypeSymbol::ErrorType;
			break;
		}

		return make_shared<BoundLiteralExpression>(syntax, ty, constant);
	}

	shared_ptr<BoundExpression> Binder::BindNameExpression(shared_ptr<Syntax::NameExpressionSyntax> syntax)
	{
		shared_ptr<Symbol::VariableSymbol> symbol = mScope->GetVariableSymbol(syntax->GetToken()->GetText());
		if (!symbol)
		{
			mDiagnosticBag->ReportUnimplimentedError(syntax->GetToken());
			return make_shared<BoundErrorExpression>(syntax);
		}

		return make_shared<BoundVariableExpression>(syntax, symbol);
	}

	#pragma endregion


	shared_ptr<DiagnosticBag> Binder::GetDiagnosticBag()
	{ return mDiagnosticBag; }
}