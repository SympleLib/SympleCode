/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#pragma once

#include "sy/Token.h"

namespace sy {
	std::vector<Token> lex(FileId fileId, std::string_view src);
} // sy