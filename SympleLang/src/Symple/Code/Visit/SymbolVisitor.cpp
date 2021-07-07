#include "Symple/Code/Visit/Visitor.h"

#include <iostream>
#include "Symple/Code/Parse/Facts.h"

namespace Symple::Code
{
	SymbolVisitor::SymbolVisitor(GlobalRef<CompilationUnitAst> unit)
		: m_Unit(unit) {}

	void SymbolVisitor::Mangle(GlobalRef<Function> fn)
	{
		std::stringstream ss;
		switch (fn->m_Call)
		{
		case TokenKind::CCallKeyword:
		case TokenKind::StdCallKeyword:
			ss << fn->m_Name->Text;
			break;
		case TokenKind::SyCallKeyword:
			ss << "Sy$" << fn->m_Name->Text;
			for (auto param : fn->m_Params)
			{
				auto ty = param->m_Type->Type;
				ss << "$" << ty->MangledName;

				if (param->m_Name)
				{
					std::stringstream ss;
					ss << "Sy$" << param->m_Name->Text << "$" << m_Depths.size();
					param->m_MangledName = ss.str();
				}
			}
			break;
		case TokenKind::SycCallKeyword:
			ss << "Syc$" << fn->m_Name->Text;
			for (auto param : fn->m_Params)
			{
				auto ty = param->m_Type->Type;
				ss << "$" << ty->MangledName;

				if (param->m_Name)
				{
					std::stringstream ss;
					ss << "Sy$" << param->m_Name->Text << m_Depths.size();
					param->m_MangledName = ss.str();
					m_Names.push_back(param);
				}
			}
			break;
		}
		
		fn->m_MangledName = ss.str();
		if (fn->m_MangledName == "Syc$Main$2char$int")
			fn->m_Main = true;
	}

	void SymbolVisitor::Mangle(GlobalRef<ParameterAst> param)
	{
		std::stringstream ss;
		ss << "Sy$" << param->m_Name->Text << "$" << 1;
		param->m_MangledName = ss.str();
	}

	void SymbolVisitor::Mangle(GlobalRef<VariableDeclarationAst> var)
	{
		std::stringstream ss;
		var->m_Depth = m_Depths.size();
		ss << "Sy$" << var->m_Name->Text << "$" << var->m_Depth;
		var->m_MangledName = ss.str();
	}

	void SymbolVisitor::Visit(ErrorList *errorList)
	{
		m_ErrorList = errorList;

		try
		{
			for (auto member : m_Unit->m_Members)
			{
				switch (member->Kind)
				{
				case AstKind::Function:
				{
					auto fn = Cast<FunctionAst>(member);
					if (fn->m_Name->Text == "Main")
						fn->m_Call = TokenKind::SycCallKeyword;
					for (auto mod : fn->m_Mods)
						if (TokenFacts::IsFuncMod(mod->Kind))
							fn->m_Call = mod->Kind;
					Mangle(fn);

					for (auto param : fn->m_Params)
						Mangle(param);
					m_Names.push_back(fn);
					break;
				}
				case AstKind::ExternFunction:
				{
					auto fn = Cast<ExternFunctionAst>(member);
					if (fn->m_Name->Text == "Main")
						fn->m_Call = TokenKind::SycCallKeyword;
					for (auto mod : fn->m_Mods)
						if (TokenFacts::IsFuncMod(mod->Kind))
							fn->m_Call = mod->Kind;
					Mangle(fn);
					m_Names.push_back(fn);
					break;
				}
				}
			}

			for (auto member : m_Unit->m_Members)
			{
				switch (member->Kind)
				{
				case AstKind::Function:
				{
					m_Depths.push_back(m_Names.size());
					auto fn = Cast<FunctionAst>(member);

					for (auto param : fn->m_Params)
						m_Names.push_back(param);
					Visit(fn->m_Body);
					m_Depths.pop_back();
					break;
				}
				}
			}
		}
		catch (const GlobalRef<const ErrorMessage> &)
		{}
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
			for (auto decl : var->m_Decls)
				Visit(decl);
			break;
		}
		case AstKind::VariableDeclaration:
		{
			auto var = Cast<VariableDeclarationAst>(stmt);
			Mangle(var);
			m_Names.push_back(var);
			if (var->m_Init)
				Visit(var->m_Init);
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
			Visit(callExpr->m_Func);
			for (auto arg : callExpr->m_Args)
				Visit(arg);
			break;
		}
		case AstKind::NameExpression:
		{
			auto nameExpr = Cast<NameExpressionAst>(expr);
			for (uint32 i = m_Names.size(); i; i--)
				if (m_Names[i - 1]->Name && m_Names[i - 1]->Name->Text == nameExpr->m_Name->Text)
				{
					nameExpr->m_Symbol = m_Names[i - 1];
					nameExpr->m_Depth = m_Depths.size();
					for (uint32 depth = 0; depth < m_Depths.size(); depth++)
						if (m_Depths[depth] >= i - 1)
							nameExpr->m_Depth = depth;
					break;
				}
			if (!nameExpr->m_Symbol)
				throw m_ErrorList->ReportUnresolvedSymbol(nameExpr->m_Name);
			break;
		}
		case AstKind::PunExpression:
		{
			auto cexpr = Cast<PunExpressionAst>(expr);
			Visit(cexpr->m_Value);
			break;
		}
		case AstKind::CastExpression:
		{
			auto cexpr = Cast<CastExpressionAst>(expr);
			Visit(cexpr->m_Value);
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