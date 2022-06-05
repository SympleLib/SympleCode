/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "sy/Air.h"

namespace sy {
	class Sema {
	private:
		ast::Module module;

		air::Project project;
		air::ScopeId currentScopeId = 0;

	public:
		Sema(ast::Module module);

		air::Project check();

	private:
		// we own the unique_ptrs, so this is 'safe'
		std::unique_ptr<air::Stmt> check(ast::Stmt *);

		air::TypeId check(ast::Type *);

		air::VarId check(ast::Var *);
		std::unique_ptr<air::Func> check(ast::Func *);
		air::VarId check(ast::Param &);

		std::unique_ptr<air::Expr> check(ast::Expr *);
		std::unique_ptr<air::BinOp> check(ast::BinOp *);
		std::unique_ptr<air::Num> check(ast::Num *);

		air::VarId addVarToCurrentScope(std::unique_ptr<air::Var>);
	};
}