#pragma once

#include "SympleCode/Tree/Tree.hpp"

class BinaryTree
{
private:
	const Tree& mLVal, mRVal;
public:
	BinaryTree(const Tree& left, const Tree& right);

	virtual int64_t Eval() = NULL;
};