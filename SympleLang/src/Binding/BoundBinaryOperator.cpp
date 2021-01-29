#include "SympleCode/Binding/BoundBinaryOperator.h"

namespace Symple::Binding
{
	BoundBinaryOperator::BoundBinaryOperator(Kind kind, shared_ptr<Type> leftType, shared_ptr<Type> rightType, shared_ptr<Type> type)
		: mKind(kind), mLeftType(leftType), mRightType(rightType), mType(type)
	{}

	void BoundBinaryOperator::Print(std::ostream& os, std::string_view indent, bool last, std::string_view label)
	{
		Syntax::Node::PrintIndent(os, indent, last, label);
		os << "Binary " << KindMap[GetKind()];
	}

	void BoundBinaryOperator::PrintShort(std::ostream& os = std::cout)
	{
		os << '(' << KindMap[GetKind()] << ')';
	}

	BoundBinaryOperator::Kind BoundBinaryOperator::GetKind()
	{ return mKind; }

	shared_ptr<Type> BoundBinaryOperator::GetLeftType()
	{ return mLeftType; }

	shared_ptr<Type> BoundBinaryOperator::GetRightType()
	{ return mRightType; }

	shared_ptr<Type> BoundBinaryOperator::GetType()
	{ return mType; }
}