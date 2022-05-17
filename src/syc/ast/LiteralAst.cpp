/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "syc/ast/LiteralAst.h"

using namespace syc;

void IntLiteralAst::print(std::ostream &os, std::string indent, std::string_view label, bool last) const {
	printIndent(os, indent, label, last);
	os << "[int] " << value;

	endPrint(os, &indent, last);
}