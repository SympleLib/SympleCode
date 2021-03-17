#include "Symple/Code/Visit/Visitor.h"

#include <iostream>

namespace Symple::Code
{
	SymbolVisitor::SymbolVisitor(GlobalRef<CompilationUnitAst> unit)
		: m_Unit(unit) {}

	void SymbolVisitor::Visit()
	{
		for (auto member : m_Unit->m_Members)
		{
			if (member->Kind == AstKind::Function)
			{
				auto fn = Cast<FunctionAst>(member);
				Visit(fn->m_Body);
				std::stringstream ss;
				ss << "_Sy$" << fn->m_Name->Text << "$Func";
				fn->m_MangledName = std::move(ss.str());
			}
		}
	}

	void SymbolVisitor::Visit(GlobalRef<StatementAst> stmt)
	{
		switch (stmt->Kind)
		{
		case AstKind::BlockStatement:
			for (auto piece : Cast<BlockStatementAst>(stmt)->m_Stmts)
				Visit(piece);
			break;
		case AstKind::ReturnStatement:
			Visit(Cast<ReturnStatementAst>(stmt)->m_Value);
			break;
		case AstKind::ExpressionStatement:
			Visit(Cast<ExpressionStatementAst>(stmt)->m_Expr);
			break;
		}
	}

	void SymbolVisitor::Visit(GlobalRef<ExpressionAst> expr)
	{
		switch (expr->Kind)
		{
		case AstKind::CallExpression:
		{
			auto callExpr = Cast<CallExpressionAst>(expr);

			bool valid = false;
			for (auto member : m_Unit->m_Members)
				if (member->Kind == AstKind::Function)
					if (Cast<FunctionAst>(member)->m_Name->Text == callExpr->m_Name->Text)
					{
						callExpr->m_Func = Cast<FunctionAst>(member);
						valid = true;
						break;
					}
			if (!valid)
				std::cerr << "Symbol '" << callExpr->m_Name->Text << "' does not exist!\n";
			break;
		}
		case AstKind::UnaryExpression:
			Visit(Cast<UnaryExpressionAst>(expr)->m_Operand);
			break;
		case AstKind::BinaryExpression:
		{
			auto binExpr = Cast<BinaryExpressionAst>(expr);
			Visit(binExpr->m_Left);
			Visit(binExpr->m_Right);
			break;
		}
		}
	}
}