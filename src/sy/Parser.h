/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "sy/Ast.h"

namespace sy {
	class Parser {
	private:
		FileId fileId;
		std::vector<Token> tokens;
		size_t idx = 0;
		std::vector<std::unique_ptr<ast::Stmt>> stmts;

	public:
		Parser(FileId fileId, std::vector<Token> &&tokens);

		std::vector<std::unique_ptr<ast::Stmt>> parse();

	private:
		std::unique_ptr<ast::Stmt> parseStmt();

		std::unique_ptr<ast::Type> parseType();

		std::unique_ptr<ast::Stmt> parseVarOrFunc();
		std::unique_ptr<ast::Var> parseVar(std::unique_ptr<ast::Type> type, Token &name);
		std::unique_ptr<ast::Func> parseFunc(std::unique_ptr<ast::Type> type, Token &name);
		ast::Param parseParam();

		std::unique_ptr<ast::Expr> parseExpr();
		std::unique_ptr<ast::Expr> parseBinOp(uint8_t precedence = 0);
		
		std::unique_ptr<ast::Expr> parsePrimary();
		std::unique_ptr<ast::Num> parseNum();

		uint8_t getBinOpPrecedence(Token::Kind kind);

		bool atEof();
	};
} // sy
