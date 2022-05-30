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
		// we own the unique_ptrs, so this is 'safe'
		std::unique_ptr<air::Stmt> check(ast::Stmt *);

		air::TypeId check(ast::Type *);

		std::unique_ptr<air::Func> check(ast::Func *);

		std::unique_ptr<air::Expr> check(ast::Expr *);
		std::unique_ptr<air::BinOp> check(ast::BinOp *);
		std::unique_ptr<air::Num> check(ast::Num *);
	};
}