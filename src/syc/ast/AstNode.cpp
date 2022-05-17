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