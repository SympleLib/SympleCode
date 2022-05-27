/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include <ostream>

#include "syc/FunEnum.h"

namespace syc {
	enum class AstKind: uint8_t {
#define ASTKIND(x) x,
#include "syc/ast/AstKind.h"
#undef ASTKIND

		Count,
	};

	constexpr const char *AstKindNames[(uint8_t)AstKind::Count] {
#define ASTKIND(x) #x,
#include "syc/ast/AstKind.h"
#undef ASTKIND
	};

	ENUM_NAME_FUNC(AstKind);

	class AstNode {
	public:
		friend std::ostream &operator <<(std::ostream &, const AstNode &);

		virtual void print(std::ostream &os, std::string indent = "", std::string_view label = "", bool last = true) const = 0;
		virtual AstKind getKind() const {
			return AstKind::None;
		}

		const char *getName() const {
			return getAstKindName(getKind());
		}

	protected:
		void printIndent(std::ostream &os, std::string_view indent, std::string_view label, bool last) const;

		void endPrint(std::ostream &os, std::string *indent, bool last) const;
	};
} // syc
