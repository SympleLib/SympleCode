#include "Symple/Code/Visit/Visitor.h"

namespace Symple::Code
{
	TypeVisitor::TypeVisitor(GlobalRef<CompilationUnitAst> unit)
		: m_Unit(unit) {}

	void TypeVisitor::Visit(ErrorList *errorList)
	{
		m_ErrorList = errorList;

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
			else if (litExpr->m_Literal->Is(TokenKind::Char))
				litExpr->m_Type = MakeRef<Type>(TypeKind::Char, 0, false);
			else if (litExpr->m_Literal->Is(TokenKind::String))
				litExpr->m_Type = MakeRef<Type>(TypeKind::Char, 1, false);
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
		case AstKind::PunExpression:
		{
			auto cexpr = Cast<PunExpressionAst>(expr);
			Visit(cexpr->m_Value);
			expr->m_Type = cexpr->m_TypeAst->m_Type;
			break;
		}
		case AstKind::CastExpression:
		{
			auto cexpr = Cast<CastExpressionAst>(expr);
			Visit(cexpr->m_Value);
			expr->m_Type = cexpr->m_TypeAst->m_Type;
			break;
		}
		case AstKind::CallExpression:
		{
			auto cexpr = Cast<CallExpressionAst>(expr);
			Visit(cexpr->m_Func);
			for (auto arg : cexpr->m_Args)
				Visit(arg);
			expr->m_Type = cexpr->m_Func->m_Type;
			break;
		}
		case AstKind::UnaryExpression:
		{
			auto unExpr = Cast<UnaryExpressionAst>(expr);
			Visit(unExpr->m_Operand);
			expr->m_Type = unExpr->m_Operand->m_Type;

			// De-reference
			if (unExpr->m_Operator->Is(TokenKind::Star))
				expr->m_Type = unExpr->m_Operand->m_Type->Deref();
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
		case AstKind::BuiltinExpression:
		{
			auto macro = Cast<BuiltinExpressionAst>(expr);
			for (auto arg : macro->Arguments)
				Visit(arg);
			break;
		}
		}
	}
}