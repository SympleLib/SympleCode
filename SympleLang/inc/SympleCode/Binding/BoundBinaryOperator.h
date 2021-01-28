#pragma once

#include "SympleCode/Syntax/Token.h"

#include "SympleCode/Binding/Type.h"

namespace Symple::Binding
{
	class BoundBinaryOperator
	{
	public: enum Kind : unsigned;
	private:
		Kind mKind;
		shared_ptr<Type> mLeftType, mRightType, mType;
	public:
		BoundBinaryOperator(Kind kind, shared_ptr<Type> leftType, shared_ptr<Type> rightType, shared_ptr<Type> type)
			: mKind(kind), mLeftType(leftType), mRightType(rightType), mType(type)
		{}

		Kind GetKind()
		{ return mKind; }

		shared_ptr<Type> GetLeftType()
		{ return mLeftType; }

		shared_ptr<Type> GetRightType()
		{ return mRightType; }

		shared_ptr<Type> GetType()
		{ return mType; }
	public:
		enum Kind : unsigned
		{
			Addition,
			Subtraction,
			Multiplication,
			Division,
		};
	};
}