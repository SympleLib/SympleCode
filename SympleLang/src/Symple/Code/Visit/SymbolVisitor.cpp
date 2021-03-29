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
			switch (member->Kind)
			{
			case AstKind::Function:
			{
				m_Depths.push_back(m_Names.size());
				auto fn = Cast<FunctionAst>(member);
				m_Names.push_back(fn);
				for (auto param : fn->m_Params)
					if (param->Name)
					{
						std::stringstream ss;
						ss << "_Sy$" << param->Name->Text << "$Var$" << m_Depths.size();
						param->m_MangledName = std::move(ss.str());
						m_Names.push_back(param);
					}

				Visit(fn->m_Body);
				m_Depths.pop_back();

				std::stringstream ss;
				ss << "_Sy$" << fn->m_Name->Text << "$Func";
				fn->m_MangledName = std::move(ss.str());
				m_Names.push_back(fn);
				break;
			}
			case AstKind::ExternFunction:
			{
				m_Depths.push_back(m_Names.size());
				auto fn = Cast<ExternFunctionAst>(member);
				m_Names.push_back(fn);
				for (auto param : fn->m_Params)
					if (param->Name)
					{
						std::stringstream ss;
						ss << "_Sy$" << param->Name->Text << "$Var$" << m_Depths.size();
						param->m_MangledName = std::move(ss.str());
						m_Names.push_back(param);
					}

				m_Depths.pop_back();

				std::stringstream ss;
				ss << "_Sy$" << fn->m_Name->Text << "$Func";
				fn->m_MangledName = std::move(ss.str());
				m_Names.push_back(fn);
				break;
			}
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
			std::stringstream ss;
			var->m_Depth = m_Depths.size();
			ss << "_Sy$" << var->Name->Text << "$Var$" << var->m_Depth;
			var->m_MangledName = std::move(ss.str());
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
			Visit(Cast<CallExpressionAst>(expr)->m_Func);
			break;
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