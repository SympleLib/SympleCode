#include "Symple/Code/Visit/Visitor.h"

namespace Symple::Code
{
	TypeVisitor::TypeVisitor(GlobalRef<CompilationUnitAst> unit)
		: m_Unit(unit) {}

	void TypeVisitor::Visit()
	{
		for (auto member : m_Unit->m_Members)
		{
			if (member->Kind == AstKind::Function)
			{
				m_Func = Cast<FunctionAst>(member);
				Visit(m_Func->m_Body);
			}
		}
	}

	void TypeVisitor::Visit(GlobalRef<StatementAst> stmt)
	{
		switch (stmt->Kind)
		{
		case AstKind::BlockStatement:
			for (auto piece : Cast<BlockStatementAst>(stmt)->m_Stmts)
				Visit(piece);
			break;
		case AstKind::ReturnStatement:
		{
			auto RetStmt = Cast<ReturnStatementAst>(stmt);
			auto val = RetStmt->m_Value;
			Visit(val);

			auto cast = MakeRef<CastExpressionAst>(WeakRef<Token>(), m_Func->m_Type, WeakRef<Token>(), val);
			cast->m_Type = cast->m_TypeAst->m_Type;
			RetStmt->m_Value = cast;
			break;
		}
		case AstKind::ExpressionStatement:
			Visit(Cast<ExpressionStatementAst>(stmt)->m_Expr);
			break;
		case AstKind::VariableStatement:
		{
			auto var = Cast<VariableStatementAst>(stmt);
			auto val = var->m_Init;
			if (val)
			{
				Visit(val);
				auto cast = MakeRef<CastExpressionAst>(WeakRef<Token>(), var->m_Type, WeakRef<Token>(), val);
				cast->m_Type = cast->m_TypeAst->m_Type;
				var->m_Init = cast;
			}
			break;
		}
		}
	}

	void TypeVisitor::Visit(GlobalRef<ExpressionAst> expr)
	{
		switch (expr->Kind)
		{
		case AstKind::LiteralExpression:
		{
			auto litExpr = Cast<LiteralExpressionAst>(expr);
			auto lit = litExpr->m_Literal->Text;
			if (lit.find('.') != std::string_view::npos)
				litExpr->m_Type = MakeRef<Type>(TypeKind::Float, 0, false);
			break;
		}
		case AstKind::ParenthasizedExpression:
		{
			auto parenExpr = Cast<ParenthasizedExpressionAst>(expr);
			Visit(parenExpr->m_Expr);
			expr->m_Type = parenExpr->m_Expr->m_Type;
			break;
		}
		case AstKind::NameExpression:
		{
			auto nameExpr = Cast<NameExpressionAst>(expr);
			nameExpr->m_Type = nameExpr->m_Symbol->Type->Type;
			break;
		}
		case AstKind::CastExpression:
			expr->m_Type = Cast<CastExpressionAst>(expr)->m_TypeAst->m_Type;
			break;
		case AstKind::CallExpression:
			expr->m_Type = Cast<CallExpressionAst>(expr)->m_Func->m_Type;
			break;
		case AstKind::UnaryExpression:
		{
			auto unExpr = Cast<UnaryExpressionAst>(expr);
			Visit(unExpr->m_Operand);
			expr->m_Type = unExpr->m_Operand->m_Type;
			break;
		}
		case AstKind::BinaryExpression:
		{
			auto binExpr = Cast<BinaryExpressionAst>(expr);
			Visit(binExpr->m_Left);
			Visit(binExpr->m_Right);
			binExpr->m_Type = binExpr->m_Left->m_Type;
			break;
		}
		}
	}
}