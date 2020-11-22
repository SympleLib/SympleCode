#pragma once

#include <cstdint>

#include <string>

#include "SympleCode/Tree/Tree.hpp"
#include "SympleCode/Token.hpp"
#include "SympleCode/Type.hpp"

using ASTToken = const std::string&;

#define AST_UNKNOWN    (ASTToken)"???"

#define AST_EXPR       (ASTToken)"Expression"
#define AST_ID         (ASTToken)"Identifier"
#define AST_COMMENT    (ASTToken)"Comment"

#define AST_TYPE       (ASTToken)"Type"
#define AST_RETURN     (ASTToken)"Return"
#define AST_VALUE      (ASTToken)"Value"
#define AST_LVALUE     (ASTToken)"LValue"
#define AST_RVALUE     (ASTToken)"RValue"
#define AST_CONSTANT   (ASTToken)"Constant"

#define AST_FUNC_DECL  (ASTToken)"Function Decleration"
#define AST_FUNC_CALL  (ASTToken)"Function Call"
#define AST_PARAM      (ASTToken)"Parameter"
#define AST_PARAMS     (ASTToken)"Parameters"
#define AST_BODY       (ASTToken)"Body"

#define AST_VAR_DECL   (ASTToken)"Varieble Declaration"
#define AST_VAR_VAL    (ASTToken)"Varieble Value"
#define AST_NAME       (ASTToken)"Name"

#define AST_GOTO       (ASTToken)"Goto"
#define AST_LABEL      (ASTToken)"Label"

#define AST_IF         (ASTToken)"If"
#define AST_ELSE       (ASTToken)"Else"
#define AST_WHILE      (ASTToken)"While"
#define AST_TERNARY    (ASTToken)"Ternary"
#define AST_COND       (ASTToken)"Condition"
#define AST_THEN       (ASTToken)"Then"

#define AST_BIN        (ASTToken)"Binary Expression"
#define AST_OP         (ASTToken)"Operator"
#define AST_ADD        (ASTToken)"Add"
#define AST_SUB        (ASTToken)"Subtract"
#define AST_MULT       (ASTToken)"Multiply"
#define AST_DIV        (ASTToken)"Divide"

#define AST_UN         (ASTToken)"Unary Expression"
#define AST_POS        (ASTToken)"Positive"
#define AST_NEG        (ASTToken)"Negative"

#define AST_PAREN      (ASTToken)"Parentasis Expression"
#define AST_LPAREN     (ASTToken)"Left Parentasis"
#define AST_RPAREN     (ASTToken)"Right Parentasis"

namespace Symple::AST
{
	Branch Null();

	Branch Id(const TokenInfo& tok);

	Branch FuncDecl(const Type& ret, const std::string& name, const Branch& params, const Branch& body);
	Branch FuncCall(const std::string& name, const Branch& params);
	Branch Param(const Type& type, const std::string& name);
	Branch Param(const Type& type);

	Branch VarDecl(Type type, const std::string& name, Branch value);
	Branch VarVal(Type type, const std::string& name);

	Branch If(const Branch& cond, const Branch& then, const Branch& elze);

	Branch Constant(const Type& type, int value);

	Branch Comment(const TokenInfo& comment);

	Branch Set(const Type& type, const TokenInfo& lvalue, const Branch& rvalue);

	Branch BinExpr(const Type& type, const TokenInfo& op, const Branch& lvalue, const Branch& rvalue);
	Branch UnExpr(const Type& type, const TokenInfo& op, const Branch& value);
	Branch ParenExpr(const Type& type, const TokenInfo& lvalue, const Branch& expr, const TokenInfo& rvalue);

	Branch While(const Branch& cond, const Branch& then);

	Branch Ternary(Type type, const Branch& cond, const Branch& then, const Branch& elze);

	Branch Return(const Branch& value);

	Branch Goto(const std::string& label);
	Branch Label(const std::string& label);

	void AssertLValue(const Branch& branch);
}