/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include <vector>
#include "syc/parse/Token.h"
#include "syc/parse/ParseScope.h"
#include "syc/ast/Ast.h"

namespace syc {
	class Parser {
	public:
		std::vector<Token> tokens;
		size_t pos;

		std::vector<AstNode *> ast;

		ParseScope globalScope;
		ParseScope *scope;

	public:
		Parser() = default;

		std::vector<AstNode *> Parse(const std::vector<Token> &tokens);

	private:
		AstNode *ParseNode();

		TypeAst *ParseType();
		PrimitiveTypeAst *ParsePrimitiveType();

		StmtAst *ParseStmt();
		ReturnStmtAst *ParseReturnStmt();
		VariableStmtAst *ParseVariableStmt();

		ExprAst *ParseExpr();
		ExprAst *ParseBinaryExpr(ExprAst *left = nullptr);

		ExprAst *ParsePrimaryExpr();

		static BinaryOp getBinaryOperator(TokenKind kind);

		Token next();
		Token match(TokenKind);
		Token peek(int64_t offset = 0) const;
	};
} // syc
