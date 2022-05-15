/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include <vector>
#include "syc/Token.h"
#include "syc/ast/Ast.h"

namespace syc {
	class Parser {
	public:
		std::vector<Token> tokens;
		size_t pos;

		std::vector<AstNode *> ast;

	public:
		Parser() = default;

		std::vector<AstNode *> Parse(const std::vector<Token> &tokens);

	private:
		ExprAst *ParseExpr();
		ExprAst *ParseBinExpr(ExprAst *left = nullptr);
		LiteralAst *ParseLiteral();

		static BinaryOperator getBinaryOperator(TokenKind kind);

		Token next();
		Token peek(int64_t offset = 0) const;
	};
} // syc
