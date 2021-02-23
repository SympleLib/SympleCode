#include "SympleCode/Binding/Binder.h"

#include <string>

#include "SympleCode/Syntax/TypeReferenceSyntax.h"
#include "SympleCode/Syntax/ParenthesizedExpressionSyntax.h"

#include "SympleCode/Symbol/TypeSymbol.h"

#include "SympleCode/Binding/BoundDefaultExpression.h"
#include "SympleCode/Binding/BoundConstantExpression.h"
#include "SympleCode/Binding/BoundImplicitCastExpression.h"
#include "SympleCode/Binding/BoundFunctionPointer.h"

#include "SympleCode/Compiler.h"

namespace Symple::Binding
{
	void Binder::BeginScope()
	{ mScope = make_shared<BoundScope>(mScope); }

	void Binder::EndScope()
	{ mScope = mScope->GetBase(); }

	std::unordered_map<std::string, shared_ptr<BoundCompilationUnit>> Binder::sImportedSymbols;

	shared_ptr<BoundCompilationUnit> Binder::BindImport(shared_ptr<Syntax::ImportStatementSyntax> syntax)
	{
		std::string path = "sy/inc/";
		path += syntax->GetImport()->GetText();
		for (auto symbol : sImportedSymbols)
			if (path == symbol.first)
			{
				auto unit = symbol.second;

				for (auto fn : unit->GetFunctions())
					mFunctions.push_back({ fn.first, nullptr });
				return unit;
			}
		for (auto symbol : Compiler::sLibraries)
			if (path == symbol)
				return make_shared<BoundCompilationUnit>(syntax, FunctionMap());
		
		if (_access(path.c_str(), 0) != -1)
		{
			Util::ConsoleColor col = Util::GetConsoleColor();
			Util::SetConsoleColor(Util::Cyan);
			spdlog::info("Import '{}'", path);
			Util::SetConsoleColor(col);
			unique_ptr<Symple::Compiler> compiler = make_unique<Symple::Compiler>((char*)path.c_str());
			compiler->Lex();
			compiler->Parse();
			compiler->Bind();
			compiler->Emit();
			compiler->Compile();
			col = Util::GetConsoleColor();
			Util::SetConsoleColor(Util::Cyan);
			spdlog::info("Imported '{}'", path);
			Util::SetConsoleColor(col);

			auto unit = compiler->mTree;
			Symple::Compiler::sUnits.push_back(compiler->mAsmPath);
			sImportedSymbols.insert({ path, unit });

			for (auto fn : unit->GetFunctions())
				mFunctions.push_back({ fn.first, nullptr });
			return unit;
		}
		else
		{
			Compiler::sLibraries.push_back(std::string(syntax->GetImport()->GetText()));
			return make_shared<BoundCompilationUnit>(syntax, FunctionMap());
		}
	}

	shared_ptr<BoundCompilationUnit> Binder::BindSymbols(shared_ptr<Syntax::TranslationUnitSyntax> unit)
	{
		mCompilationUnit = unit;
		mFunctions.clear();
		mScope.reset();
		BeginScope();

		for (auto member : mCompilationUnit->GetMembers())
			BindMemberSymbol(member);

		EndScope();

		return make_shared<BoundCompilationUnit>(unit, mFunctions);
	}

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

	shared_ptr<Symbol::LabelSymbol> Binder::BindLabelSymbol(shared_ptr<Syntax::LabelSyntax> syntax)
	{
		shared_ptr<Symbol::LabelSymbol> label = make_shared<Symbol::LabelSymbol>(syntax->GetLabel()->GetText());
		mLabels.push_back(label);
		return label;
	}

	shared_ptr<Symbol::FunctionSymbol> Binder::BindFunction(shared_ptr<Syntax::FunctionDeclarationSyntax> syntax)
	{
		shared_ptr<Symbol::TypeSymbol> ty = BindType(syntax->GetType());
		std::string_view name = syntax->GetName()->GetText();
		Symbol::ParameterList params;
		for (auto param : syntax->GetParameters())
			params.push_back(BindParameter(param));

		Symbol::FunctionSymbol::CallingConvention conv = Symbol::FunctionSymbol::CDecl;
		bool dll = false, isGlobal = true;
		for (auto mod : syntax->GetModifiers())
			switch (mod->GetKind())
			{
			case Syntax::Token::CDeclKeyword:
				conv = Symbol::FunctionSymbol::CDecl;
				break;
			case Syntax::Token::StdCallKeyword:
				conv = Symbol::FunctionSymbol::StdCall;
				break;
			case Syntax::Token::DllExportKeyword:
				dll = true;
				break;
			case Syntax::Token::DllImportKeyword:
				mDiagnosticBag->ReportUnexpectedDllImportBody(mod);
				break;
			case Syntax::Token::LocalKeyword:
				isGlobal = false;
				break;
			case Syntax::Token::GlobalKeyword:
				isGlobal = true;
				break;
			}

		shared_ptr<Symbol::FunctionSymbol> symbol = make_shared<Symbol::FunctionSymbol>(ty, name, params, conv, false, dll, isGlobal);

		BeginScope();
		for (auto param : symbol->GetParameters())
			mScope->DeclareVariable(param);
		shared_ptr<BoundStatement> body = BindStatement(syntax->GetBody());
		EndScope();

		for (auto promise : mGotoPromises)
		{
			for (auto label : mLabels)
				if (label->GetLabel() == promise.GetPrompt())
				{
					promise.Complete(label);
					break;
				}
			if (promise.IsBroken())
				abort();
		}

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

		Symbol::FunctionSymbol::CallingConvention conv = Symbol::FunctionSymbol::CDecl;
		bool dll = false, isGlobal = true;
		for (auto mod : syntax->GetModifiers())
			switch (mod->GetKind())
			{
			case Syntax::Token::CDeclKeyword:
				conv = Symbol::FunctionSymbol::CDecl;
				break;
			case Syntax::Token::StdCallKeyword:
				conv = Symbol::FunctionSymbol::StdCall;
				break;
			case Syntax::Token::DllImportKeyword:
				dll = true;
				break;
			case Syntax::Token::LocalKeyword:
				isGlobal = false;
				break;
			case Syntax::Token::GlobalKeyword:
				isGlobal = true;
				break;
			}

		shared_ptr<Symbol::FunctionSymbol> symbol = make_shared<Symbol::FunctionSymbol>(ty, name, params, conv, dll, false, isGlobal);

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

	shared_ptr<Symbol::Symbol> Binder::BindMemberSymbol(shared_ptr<Syntax::MemberSyntax> syntax)
	{
		switch (syntax->GetKind())
		{
		case Syntax::Node::ExternFunction:
			return BindExternFunction(dynamic_pointer_cast<Syntax::ExternFunctionSyntax>(syntax));
		case Syntax::Node::FunctionDeclaration:
			return BindFunction(dynamic_pointer_cast<Syntax::FunctionDeclarationSyntax>(syntax));
		case Syntax::Node::ImportStatement:
			BindImport(dynamic_pointer_cast<Syntax::ImportStatementSyntax>(syntax));
			return make_shared<Symbol::Symbol>();
		}
	}

	shared_ptr<Node> Binder::BindMember(shared_ptr<Syntax::MemberSyntax> syntax)
	{
		shared_ptr<Node> result = BindMemberInternal(syntax);
		mGotoPromises.clear();
		mLabels.clear();
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
		case Syntax::Node::ImportStatement:
			BindImport(dynamic_pointer_cast<Syntax::ImportStatementSyntax>(syntax));
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
		case Syntax::Node::Label:
			return BindLabel(dynamic_pointer_cast<Syntax::LabelSyntax>(syntax));
		case Syntax::Node::NativeStatement:
			return BindNativeCode(dynamic_pointer_cast<Syntax::NativeStatementSyntax>(syntax));
		case Syntax::Node::IfStatement:
			return BindIfStatement(dynamic_pointer_cast<Syntax::IfStatementSyntax>(syntax));
		case Syntax::Node::GotoStatement:
			return BindGotoStatement(dynamic_pointer_cast<Syntax::GotoStatementSyntax>(syntax));
		case Syntax::Node::BlockStatement:
			return BindBlockStatement(dynamic_pointer_cast<Syntax::BlockStatementSyntax>(syntax));
		case Syntax::Node::ReturnStatement:
			return BindReturnStatement(dynamic_pointer_cast<Syntax::ReturnStatementSyntax>(syntax));
		case Syntax::Node::ExpressionStatement:
			return BindExpressionStatement(dynamic_pointer_cast<Syntax::ExpressionStatementSyntax>(syntax));
		case Syntax::Node::VariableDeclaration:
			return BindVariableDeclaration(dynamic_pointer_cast<Syntax::VariableDeclarationSyntax>(syntax));
		default:
			return make_shared<BoundStatement>(syntax);
		}
	}

	shared_ptr<BoundLabel> Binder::BindLabel(shared_ptr<Syntax::LabelSyntax> syntax)
	{ return make_shared<BoundLabel>(syntax, BindLabelSymbol(syntax)); }

	shared_ptr<BoundNativeCode> Binder::BindNativeCode(shared_ptr<Syntax::NativeStatementSyntax> syntax)
	{ return make_shared<BoundNativeCode>(syntax); }

	shared_ptr<BoundIfStatement> Binder::BindIfStatement(shared_ptr<Syntax::IfStatementSyntax> syntax)
	{
		shared_ptr<BoundExpression> cond = BindExpression(syntax->GetCondition());
		shared_ptr<BoundStatement> then = BindStatement(syntax->GetThen());
		shared_ptr<BoundStatement> elze = BindStatement(syntax->GetElse());

		return make_shared<BoundIfStatement>(syntax, cond, then, elze);
	}

	shared_ptr<BoundGotoStatement> Binder::BindGotoStatement(shared_ptr<Syntax::GotoStatementSyntax> syntax)
	{
		Promise<shared_ptr<Symbol::LabelSymbol>, std::string> promise(std::string(syntax->GetLabel()->GetText()));
		mGotoPromises.push_back(promise);
		return make_shared<BoundGotoStatement>(syntax, promise);
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

	shared_ptr<BoundVariableDeclaration> Binder::BindVariableDeclaration(shared_ptr<Syntax::VariableDeclarationSyntax> syntax)
	{
		shared_ptr<Symbol::TypeSymbol> ty = BindType(syntax->GetType());
		std::string_view name = syntax->GetName()->GetText();
		shared_ptr<BoundExpression> init;
		if (syntax->GetInitializer())
			init = BindExpression(syntax->GetInitializer());

		shared_ptr<Symbol::VariableSymbol> symbol = make_shared<Symbol::VariableSymbol>(ty, name);
		mScope->DeclareVariable(symbol);
		return make_shared<BoundVariableDeclaration>(syntax, symbol, init);
	}

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
		ExpressionList args;
		if (funcSymbol)
		{
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
		else
		{
			mDiagnosticBag->ReportNoSuchFunction(syntax);
			return make_shared<BoundErrorExpression>(syntax);
		}
	}

	shared_ptr<BoundUnaryExpression> Binder::BindUnaryExpression(shared_ptr<Syntax::UnaryExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> operand = BindExpression(syntax->GetOperand());

		shared_ptr<BoundUnaryOperator> op = BoundUnaryOperator::Bind(syntax->GetOperator()->GetKind(), operand->GetType());
		if (op == BoundUnaryOperator::ErrorOperator)
			mDiagnosticBag->ReportInvalidOperation(syntax->GetOperator(), operand->GetType());

		return make_shared<BoundUnaryExpression>(syntax, op, operand);
	}

	shared_ptr<BoundExpression> Binder::BindBinaryExpression(shared_ptr<Syntax::BinaryExpressionSyntax> syntax)
	{
		shared_ptr<BoundExpression> left = BindExpression(syntax->GetLeft());
		shared_ptr<BoundExpression> right = BindExpression(syntax->GetRight());

		shared_ptr<BoundBinaryOperator> op = BoundBinaryOperator::Bind(syntax->GetOperator()->GetKind(), left->GetType(), right->GetType());
		if (op == BoundBinaryOperator::ErrorOperator)
			mDiagnosticBag->ReportInvalidOperation(syntax->GetOperator(), left->GetType(), right->GetType());
		else if (op->IsMutable() && !left->IsMutable())
			mDiagnosticBag->ReportExpectedLValue(left->GetSyntax()->GetToken());

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
		case Syntax::Token::String:
		{
			ty = Symbol::TypeSymbol::CharPointerType;
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
		shared_ptr<Symbol::VariableSymbol> varSymbol = mScope->GetVariableSymbol(syntax->GetToken()->GetText());
		if (varSymbol)
			return make_shared<BoundVariableExpression>(syntax, varSymbol);
		else
		{
			shared_ptr<Symbol::FunctionSymbol> fnSymbol = FindFunction(mFunctions, syntax->GetToken()->GetText());

			if (fnSymbol)
				return make_shared<BoundFunctionPointer>(syntax, fnSymbol);
			else
			{
				mDiagnosticBag->ReportUndeclaredIdentifier(syntax->GetToken());
				return make_shared<BoundErrorExpression>(syntax);
			}
		}
	}

	#pragma endregion


	shared_ptr<DiagnosticBag> Binder::GetDiagnosticBag()
	{ return mDiagnosticBag; }
}