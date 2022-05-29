/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "sy/Air.h"

namespace sy {
	class Sema {
	private:
		std::vector<std::unique_ptr<ast::Stmt>> parsedStmts;

		air::Project project;

	public:
		Sema(std::vector<std::unique_ptr<ast::Stmt>> &&parsedStmts);

		air::Project check();

	private:
		std::unique_ptr<air::Stmt> checkStmt(std::unique_ptr<ast::Stmt>);

		std::unique_ptr<air::Expr> checkExpr(std::unique_ptr<ast::Expr>);
		std::unique_ptr<air::BinOp> checkBinOp(std::unique_ptr<ast::BinOp>);
		std::unique_ptr<air::Num> checkNum(std::unique_ptr<ast::Num>);
	};
}