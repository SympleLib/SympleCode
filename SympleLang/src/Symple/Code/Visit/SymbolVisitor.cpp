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
				m_Depths.push_back(m_Names.size());
				auto fn = Cast<FunctionAst>(member);
				m_Names.push_back(fn);
				for (auto param : fn->m_Params)
					m_Names.push_back(param);

				Visit(fn->m_Body);
				std::stringstream ss;
				ss << "_Sy$" << fn->m_Name->Text << "$Func";
				fn->m_MangledName = std::move(ss.str());
				m_Depths.pop_back();
			}
		}
	}

	void SymbolVisitor::Visit(GlobalRef<StatementAst> stmt)
	{
		switch (stmt->Kind)
		{
		case AstKind::BlockStatement:
			m_Depths.push_back(m_Names.size());
			for (auto piece : Cast<BlockStatementAst>(stmt)->m_Stmts)
				Visit(piece);
			m_Depths.pop_back();
			break;
		case AstKind::ReturnStatement:
			Visit(Cast<ReturnStatementAst>(stmt)->m_Value);
			break;
		case AstKind::ExpressionStatement:
			Visit(Cast<ExpressionStatementAst>(stmt)->m_Expr);
			break;
		case AstKind::VariableStatement:
		{
			auto var = Cast<VariableStatementAst>(stmt);
			var->m_Depth = m_Depths.size();
			m_Names.push_back(var);
			break;
		}
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
		case AstKind::NameExpression:
		{
			auto nameExpr = Cast<NameExpressionAst>(expr);
			for (uint32 i = m_Names.size(); i; i--)
				if (m_Names[i - 1]->Name->Text == nameExpr->m_Name->Text)
				{
					nameExpr->m_Symbol = m_Names[i - 1];
					nameExpr->m_Depth = m_Depths.size();
					for (uint32 depth = 0; depth < m_Depths.size(); depth++)
						if (m_Depths[depth] >= i - 1)
							nameExpr->m_Depth = depth;
					break;
				}
			if (!nameExpr->m_Symbol)
				std::cerr << "Symbol '" << nameExpr->m_Name->Text << "' does not exist!\n";
			break;
		}
		case AstKind::ParenthasizedExpression:
			Visit(Cast<ParenthasizedExpressionAst>(expr)->m_Expr);
			break;
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