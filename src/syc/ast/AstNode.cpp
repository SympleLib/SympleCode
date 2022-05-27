/*
 * Copyright (c) 2022-present, Treidex <treidexy@gmail.com>
 *
 * MIT License
 */

#include "syc/ast/AstNode.h"

using namespace syc;

std::ostream &syc::operator <<(std::ostream &os, const AstNode &node) {
	node.print(os);
	return os;
}

void AstNode::printIndent(std::ostream &os, std::string_view indent, std::string_view label, bool last) const {
	os << indent << (last ? "L--" : "|--");
	if (!label.empty())
		os << label << ":";
	os << " (" << getName() << ") ";
}

void AstNode::endPrint(std::ostream &os, std::string *indent, bool last) const {
	os << '\n';
	*indent += last ? "   " : "|  ";
}