/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "syc/FunEnum.h"

#include <cstdint>

namespace syc {

	enum class RefTypeKind: uint8_t {
#define REFTYPE(x, y) x,
#include "syc/ast/RefTypeKind.def"

		Count,
	};

	constexpr const char *RefTypeKindNames[(uint8_t) RefTypeKind::Count] {
//#define REFTYPE(x, y) #x " '" y "'",
#define REFTYPE(x, y) y,
#include "syc/ast/RefTypeKind.def"
	};

	ENUM_NAME_FUNC(RefTypeKind);
}