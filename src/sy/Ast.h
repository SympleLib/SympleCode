/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "sy/Token.h"

namespace sy::ast {
	enum Kind {
		Unknown,
		Num,
	};

	struct Stmt {
		Span span;

		virtual Kind getKind() const;
	};

	struct Type {
	};

	struct Expr: Stmt {
		std::unique_ptr<Type> type = nullptr;
	};

	struct Num: Expr {
		uint64_t asU64;

		Num(uint64_t asU64, std::unique_ptr<Type> type, Span span);

		Kind getKind() const override;
	};
}