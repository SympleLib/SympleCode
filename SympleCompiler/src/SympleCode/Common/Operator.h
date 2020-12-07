#pragma once

namespace Symple
{
	enum class Operator
	{
		// Unary Operators
		Negative,
		Positive,

		AddOne,
		SubOne,

		// Binary Operators
		Addition,
		Subtraction,
		Multiplication,
		Division,

		BitShiftLeft,
		BitShiftRight,
		BitOr,
		BitXOr,
		BitAnd,
		BitReverse,

		LogicalNot,
		LogicalOr,
		LogicalAnd,
	};
}