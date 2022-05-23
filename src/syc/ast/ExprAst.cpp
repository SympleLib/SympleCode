/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "syc/ast/ExprAst.h"

#include "syc/ast/Ast.h"

using namespace syc;

void BinaryExprAst::print(std::ostream &os, std::string indent, std::string_view label, bool last) const {
	printExprIndent(os, indent, label, last);
	os << getBinaryOpName(op);

	endPrint(os, &indent, last);

	left->print(os, indent, "left", false);
	right->print(os, indent, "right", true);
}

void VariableExprAst::print(std::ostream &os, std::string indent, std::string_view label, bool last) const {
	printExprIndent(os, indent, label, last);
	os << var->name;

	endPrint(os, &indent, last);
}