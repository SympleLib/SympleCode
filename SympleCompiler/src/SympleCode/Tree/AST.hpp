#pragma once

#include <cstdint>

#include "SympleCode/Util/Type.hpp"

using ASTToken = const std::string&;

#define AST_TYPE      "Type"
#define AST_RETURN    "Return"
#define AST_VALUE     "Value"
#define AST_LVALUE    "LValue"
#define AST_RVALUE    "RValue"

#define AST_VAR       "Var"
#define AST_NAME      "Name"

#define AST_GOTO      "Goto"
#define AST_LABEL     "Label"

#define AST_IF        "If"
#define AST_ELSE      "Else"
#define AST_WHILE     "While"
#define AST_TERNARY   "Ternary"
#define AST_COND      "Condition"
#define AST_THEN      "Then"

#define AST_BIN       "Binary Expression"
#define AST_OP        "Operator"
#define AST_ADD       "Add"
#define AST_SUBTRACT  "Subtract"

namespace Symple::AST
{
	inline Branch Var(Type type, const std::string& name)
	{
		Branch branch(AST_VAR);
		branch.PushBranch(AST_NAME, name);
		branch.PushBranch(AST_TYPE, type);
		return branch;
	}

	inline Branch If(const Branch& cond, const Branch& then, const Branch& elze)
	{
		Branch branch(AST_IF);
		branch.PushBranch(AST_COND, cond);
		branch.PushBranch(AST_THEN, then);
		branch.PushBranch(AST_ELSE, elze);
		return branch;
	}

	inline Branch Constant(const Type& type, int value)
	{
		return { "Constant", value };
	}

	inline Branch BinExpr(const Type& type, const TokenInfo& op, const Branch& lvalue, const Branch& rvalue)
	{
		Branch branch(AST_BIN);
		branch.PushBranch(AST_TYPE, type);
		branch.PushBranch(AST_OP, op);
		branch.PushBranch(AST_LVALUE, lvalue);
		branch.PushBranch(AST_RVALUE, rvalue);
		return branch;
	}

	inline Branch While(const Branch& cond, const Branch& then)
	{
		Branch branch(AST_WHILE);
		branch.PushBranch(AST_COND, cond);
		branch.PushBranch(AST_THEN, then);
		return branch;
	}

	inline Branch Ternary(Type type, const Branch& cond, const Branch& then, const Branch& elze)
	{
		Branch branch(AST_TERNARY);
		branch.PushBranch(AST_TYPE, type);
		branch.PushBranch(AST_COND, cond);
		branch.PushBranch(AST_THEN, then);
		branch.PushBranch(AST_ELSE, elze);
		return branch;
	}

	inline Branch Return(const Branch& value)
	{
		Branch branch(AST_RETURN);
		branch.PushBranch(AST_VALUE, value);
		return branch;
	}

	inline Branch Goto(const std::string& label)
	{
		Branch branch(AST_GOTO);
		branch.PushBranch(AST_LABEL, label);
		return branch;
	}

	inline Branch Label(const std::string& label)
	{
		Branch branch(AST_LABEL);
		return branch;
	}

	inline void AssertLValue(const Branch& branch)
	{
		if (branch.Label == AST_LVALUE)
			return;
		perror("LValue Expected!");
	}
}