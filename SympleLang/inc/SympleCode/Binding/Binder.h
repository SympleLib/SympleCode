#pragma once

#include <map>

#include "SympleCode/DiagnosticBag.h"

#include "SympleCode/Syntax/TypeSyntax.h"
#include "SympleCode/Syntax/ImportStatementSyntax.h"
#include "SympleCode/Syntax/ExternFunctionSyntax.h"
#include "SympleCode/Syntax/GlobalStatementSyntax.h"
#include "SympleCode/Syntax/FunctionDeclarationSyntax.h"

#include "SympleCode/Syntax/StatementSyntax.h"
#include "SympleCode/Syntax/BlockStatementSyntax.h"

#include "SympleCode/Syntax/ExpressionSyntax.h"

#include "SympleCode/Symbol/Symbol.h"
#include "SympleCode/Symbol/FunctionSymbol.h"

#include "SympleCode/Binding/Node.h"
#include "SympleCode/Binding/BoundScope.h"
#include "SympleCode/Binding/BoundCompilationUnit.h"

#include "SympleCode/Binding/BoundLabel.h"
#include "SympleCode/Binding/BoundStatement.h"
#include "SympleCode/Binding/BoundNativeCode.h"
#include "SympleCode/Binding/BoundGotoStatement.h"
#include "SympleCode/Binding/BoundBlockStatement.h"
#include "SympleCode/Binding/BoundReturnStatement.h"
#include "SympleCode/Binding/BoundExpressionStatement.h"
#include "SympleCode/Binding/BoundVariableDeclaration.h"

#include "SympleCode/Binding/BoundExpression.h"
#include "SympleCode/Binding/BoundCallExpression.h"
#include "SympleCode/Binding/BoundErrorExpression.h"
#include "SympleCode/Binding/BoundUnaryExpression.h"
#include "SympleCode/Binding/BoundBinaryExpression.h"
#include "SympleCode/Binding/BoundLiteralExpression.h"
#include "SympleCode/Binding/BoundVariableExpression.h"

namespace Symple::Binding
{
	class Binder
	{
	private:
		static std::map<std::string, shared_ptr<BoundCompilationUnit>> sImportedSymbols;
		shared_ptr<Syntax::TranslationUnitSyntax> mCompilationUnit;
		FunctionMap mFunctions;
		Symbol::LabelList mLabels;
		std::vector<std::string> mGotoPromises;

		shared_ptr<BoundScope> mScope;
		shared_ptr<DiagnosticBag> mDiagnosticBag = make_shared<DiagnosticBag>();

		void BeginScope();
		void EndScope();

		shared_ptr<Node> BindMemberInternal(shared_ptr<Syntax::MemberSyntax>);
		shared_ptr<BoundStatement> BindStatementInternal(shared_ptr<Syntax::StatementSyntax>);
		shared_ptr<BoundExpression> BindExpressionInternal(shared_ptr<Syntax::ExpressionSyntax>);
	public:
		shared_ptr<BoundCompilationUnit> BindImport(shared_ptr<Syntax::ImportStatementSyntax>);
		shared_ptr<BoundCompilationUnit> BindSymbols(shared_ptr<Syntax::TranslationUnitSyntax>);
		shared_ptr<BoundCompilationUnit> Bind(shared_ptr<Syntax::TranslationUnitSyntax>);
		shared_ptr<Symbol::Symbol> BindMemberSymbol(shared_ptr<Syntax::MemberSyntax>);
		shared_ptr<Node> BindMember(shared_ptr<Syntax::MemberSyntax>);

		shared_ptr<Symbol::TypeSymbol> BindType(shared_ptr<Syntax::TypeSyntax>);
		shared_ptr<Symbol::LabelSymbol> BindLabelSymbol(shared_ptr<Syntax::LabelSyntax>);
		shared_ptr<Symbol::FunctionSymbol> BindFunction(shared_ptr<Syntax::FunctionDeclarationSyntax>);
		shared_ptr<Symbol::FunctionSymbol> BindExternFunction(shared_ptr<Syntax::ExternFunctionSyntax>);
		shared_ptr<Symbol::ParameterSymbol> BindParameter(shared_ptr<Syntax::VariableDeclarationSyntax>);

		shared_ptr<BoundStatement> BindStatement(shared_ptr<Syntax::StatementSyntax>);
		shared_ptr<BoundLabel> BindLabel(shared_ptr<Syntax::LabelSyntax>);
		shared_ptr<BoundNativeCode> BindNativeCode(shared_ptr<Syntax::NativeStatementSyntax>);
		shared_ptr<BoundStatement> BindGlobalStatement(shared_ptr<Syntax::GlobalStatementSyntax>);
		shared_ptr<BoundGotoStatement> BindGotoStatement(shared_ptr<Syntax::GotoStatementSyntax>);
		shared_ptr<BoundBlockStatement> BindBlockStatement(shared_ptr<Syntax::BlockStatementSyntax>);
		shared_ptr<BoundReturnStatement> BindReturnStatement(shared_ptr<Syntax::ReturnStatementSyntax>);
		shared_ptr<BoundExpressionStatement> BindExpressionStatement(shared_ptr<Syntax::ExpressionStatementSyntax>);
		shared_ptr<BoundVariableDeclaration> BindVariableDeclaration(shared_ptr<Syntax::VariableDeclarationSyntax>);

		shared_ptr<BoundExpression> BindExpression(shared_ptr<Syntax::ExpressionSyntax>);
		shared_ptr<BoundExpression> BindCallExpression(shared_ptr<Syntax::CallExpressionSyntax>);
		shared_ptr<BoundUnaryExpression> BindUnaryExpression(shared_ptr<Syntax::UnaryExpressionSyntax>);
		shared_ptr<BoundExpression> BindBinaryExpression(shared_ptr<Syntax::BinaryExpressionSyntax>);
		shared_ptr<BoundExpression> BindLiteralExpression(shared_ptr<Syntax::LiteralExpressionSyntax>);
		shared_ptr<BoundExpression> BindNameExpression(shared_ptr<Syntax::NameExpressionSyntax>);

		shared_ptr<DiagnosticBag> GetDiagnosticBag();
	};
}