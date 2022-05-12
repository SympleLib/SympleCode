/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

namespace syc {
	enum class AstKind {
		None,

		BinaryExpr,

		IntLiteral,
	};

	class AstNode {
	public:
		AstKind kind = AstKind::None;
	};
} // syc
