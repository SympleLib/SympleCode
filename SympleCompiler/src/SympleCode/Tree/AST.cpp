#include "SympleCode/Tree/AST.hpp"

namespace Symple::AST
{
	Branch Null()
	{
		return { "Null" };
	}

	Branch Id(const TokenInfo& tok)
	{
		Branch branch(AST_ID);
		branch.PushBranch(AST_NAME, tok.GetLex());
		return branch;
	}

	Branch FuncDecl(const Type& ret, const std::string& name, const Branch& params, const Branch& body)
	{
		Branch branch(AST_FUNC_DECL);
		branch.PushBranch(AST_NAME, name);
		branch.PushBranch(AST_TYPE, ret);
		branch.PushBranch(AST_PARAMS, params);
		branch.PushBranch(AST_BODY, body);
		return branch;
	}

	Branch FuncCall(const std::string& name, const Branch& params)
	{
		Branch branch(AST_FUNC_CALL);
		branch.PushBranch(AST_NAME, name);
		branch.PushBranch(AST_PARAMS, params);
		return branch;
	}

	Branch Param(const Type& type, const std::string& name)
	{
		Branch branch(AST_PARAM);
		branch.PushBranch(AST_NAME, name);
		branch.PushBranch(AST_TYPE, type);
		return branch;
	}

	Branch Param(const Type& type)
	{
		Branch branch(AST_PARAM);
		branch.PushBranch(AST_TYPE, type);
		return branch;
	}

	Branch VarDecl(Type type, const std::string& name, Branch value)
	{
		Branch branch(AST_VAR_DECL);
		branch.PushBranch(AST_NAME, name);
		branch.PushBranch(AST_TYPE, type);
		branch.PushBranch(AST_VALUE, value);
		return branch;
	}

	Branch VarVal(Type type, const std::string& name)
	{
		Branch branch(AST_VAR_VAL);
		branch.PushBranch(AST_NAME, name);
		branch.PushBranch(AST_TYPE, type);
		return branch;
	}

	Branch If(const Branch& cond, const Branch& then, const Branch& elze)
	{
		Branch branch(AST_IF);
		branch.PushBranch(AST_COND, cond);
		branch.PushBranch(AST_THEN, then);
		branch.PushBranch(AST_ELSE, elze);
		return branch;
	}

	Branch Constant(const Type& type, int value)
	{
		return { AST_CONSTANT, value };
	}

	Branch Comment(const TokenInfo& comment)
	{
		return { AST_COMMENT, comment };
	}

	Branch Set(const Type& type, const TokenInfo& lvalue, const Branch& rvalue)
	{
		Branch branch(AST_BIN);
		branch.PushBranch(AST_TYPE, type);
		branch.PushBranch(AST_LVALUE, lvalue);
		branch.PushBranch(AST_RVALUE, rvalue);
		return branch;
	}

	Branch BinExpr(const Type& type, const TokenInfo& op, const Branch& lvalue, const Branch& rvalue)
	{
		Branch branch(AST_BIN);
		branch.PushBranch(AST_TYPE, type);
		branch.PushBranch(AST_OP, op.Is(Tokens::Plus) ? AST_ADD : op.Is(Tokens::Minus) ? AST_SUB : op.Is(Tokens::Asterisk) ? AST_MULT : op.Is(Tokens::Slash) ? AST_DIV : AST_UNKNOWN);
		branch.PushBranch(AST_LVALUE, lvalue);
		branch.PushBranch(AST_RVALUE, rvalue);
		return branch;
	}

	Branch UnExpr(const Type& type, const TokenInfo& op, const Branch& value)
	{
		Branch branch(AST_UN);
		branch.PushBranch(AST_TYPE, type);
		branch.PushBranch(AST_OP, op.Is(Tokens::Plus) ? AST_POS : op.Is(Tokens::Minus) ? AST_NEG : AST_UNKNOWN);
		branch.PushBranch(AST_VALUE, value);
		return branch;
	}

	Branch ParenExpr(const Type& type, const TokenInfo& lvalue, const Branch& expr, const TokenInfo& rvalue)
	{
		Branch branch(AST_PAREN);
		branch.PushBranch(AST_TYPE, type);
		branch.PushBranch(AST_EXPR, expr);
		branch.PushBranch(AST_LVALUE, lvalue);
		branch.PushBranch(AST_RVALUE, rvalue);
		return branch;
	}

	Branch While(const Branch& cond, const Branch& then)
	{
		Branch branch(AST_WHILE);
		branch.PushBranch(AST_COND, cond);
		branch.PushBranch(AST_THEN, then);
		return branch;
	}

	Branch Ternary(Type type, const Branch& cond, const Branch& then, const Branch& elze)
	{
		Branch branch(AST_TERNARY);
		branch.PushBranch(AST_TYPE, type);
		branch.PushBranch(AST_COND, cond);
		branch.PushBranch(AST_THEN, then);
		branch.PushBranch(AST_ELSE, elze);
		return branch;
	}

	Branch Return(const Branch& value)
	{
		Branch branch(AST_RETURN);
		branch.PushBranch(AST_VALUE, value);
		return branch;
	}

	Branch Goto(const std::string& label)
	{
		Branch branch(AST_GOTO);
		branch.PushBranch(AST_LABEL, label);
		return branch;
	}

	Branch Label(const std::string& label)
	{
		Branch branch(AST_LABEL);
		return branch;
	}

	void AssertLValue(const Branch& branch)
	{
		if (branch.Label == AST_LVALUE)
			return;
		perror("LValue Expected!");
	}
}