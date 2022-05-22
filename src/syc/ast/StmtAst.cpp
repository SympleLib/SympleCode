/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "syc/ast/StmtAst.h"

#include "syc/ast/Ast.h"

using namespace syc;

void ReturnStmtAst::print(std::ostream &os, std::string indent, std::string_view label, bool last) const {
	printIndent(os, indent, label, last);

	endPrint(os, &indent, last);
	if (value)
		value->print(os, indent, "value", true);
}

void VariableStmtAst::print(std::ostream &os, std::string indent, std::string_view label, bool last) const {
	printIndent(os, indent, label, last);

	os << name;

	endPrint(os, &indent, last);

	type->print(os, indent, "type", !init);
	if (init)
		init->print(os, indent, "init", true);
}