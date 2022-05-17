/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "syc/ast/ExprAst.h"

using namespace syc;

void BinaryExprAst::print(std::ostream &os, std::string indent, std::string_view label, bool last) const {
	printIndent(os, indent, label, last);
	os << getBinaryOpName(op);

	endPrint(os, &indent, last);

	left->print(os, indent, "left", false);
	right->print(os, indent, "right", true);
}