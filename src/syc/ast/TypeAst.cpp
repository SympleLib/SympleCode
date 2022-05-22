/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "syc/ast/TypeAst.h"

using namespace syc;

void PrimitiveTypeAst::print(std::ostream &os, std::string indent, std::string_view label, bool last) const {
	printTypeIndent(os, indent, label, last);
	endPrint(os, &indent, last);
}