#include "Symple/Code/Visit/Visitor.h"

namespace Symple::Code
{
	TypeVisitor::TypeVisitor(GlobalRef<CompilationUnitAst> unit)
		: m_Unit(unit) {}

	GlobalRef<CastExpressionAst> TypeVisitor::InsertCast(GlobalRef<ExpressionAst> from, GlobalRef<const Type> to)
	{
		auto cast = MakeRef<CastExpressionAst>(WeakRef<Token>(), MakeRef<TypeAst>(to), WeakRef<Token>(), from);
		cast->m_Type = cast->m_TypeAst->m_Type;
		return cast;
	}

	GlobalRef<CastExpressionAst> TypeVisitor::InsertCast(GlobalRef<ExpressionAst> from, GlobalRef<TypeAst> to)
	{
		auto cast = MakeRef<CastExpressionAst>(WeakRef<Token>(), to, WeakRef<Token>(), from);
		cast->m_Type = cast->m_TypeAst->m_Type;
		return cast;
	}

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

			RetStmt->m_Value = InsertCast(val, m_Func->m_Type);
			break;
		}
		case AstKind::ExpressionStatement:
			Visit(Cast<ExpressionStatementAst>(stmt)->m_Expr);
			break;
		case AstKind::VariableStatement:
		{
			auto var = Cast<VariableStatementAst>(stmt);
			for (auto decl : var->m_Decls)
				Visit(decl);
			break;
		}
		case AstKind::VariableDeclaration:
		{
			auto var = Cast<VariableDeclarationAst>(stmt);
			auto val = var->m_Init;
			if (val)
			{
				Visit(val);
				var->m_Init = InsertCast(val, var->m_Type);
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
			if (litExpr->m_Literal->Is(TokenKind::Char))
				litExpr->m_Type = MakeRef<Type>(NativeType::Char, 0);
			else if (litExpr->m_Literal->Is(TokenKind::String))
				litExpr->m_Type = MakeRef<Type>(NativeType::Char, 1);
			else if (lit.find('.') != std::string_view::npos)
					litExpr->m_Type = MakeRef<Type>(NativeType::Float, 0);
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
			if (unExpr->m_Operator->Is(TokenKind::At))
				expr->m_Type = unExpr->m_Operand->m_Type->Ptr();
			else
				expr->m_Type = unExpr->m_Operand->m_Type;

			// De-reference
			if (unExpr->m_Operator->Is(TokenKind::Star))
				expr->m_Type = unExpr->m_Operand->m_Type->Deref();
			break;
		}
		case AstKind::BinaryExpression:
		{
			auto binExpr = Cast<BinaryExpressionAst>(expr);
			auto left = binExpr->m_Left, right = binExpr->m_Right;
			Visit(left);
			Visit(right);

			binExpr->m_Type = left->m_Type;
			binExpr->m_Left = InsertCast(left, binExpr->m_Type);
			binExpr->m_Right = InsertCast(right, binExpr->m_Type);
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