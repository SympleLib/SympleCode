#include "SympleCode/Binding/BoundBinaryOperator.h"

namespace Symple::Binding
{
	std::vector<shared_ptr<BoundBinaryOperator>> BoundBinaryOperator::sOperators = {
		make_shared<BoundBinaryOperator>(Syntax::Token::Plus, Addition, Type::IntType)
	};

	BoundBinaryOperator::BoundBinaryOperator(Syntax::Token::Kind tokenKind, Kind kind, shared_ptr<Type> leftType, shared_ptr<Type> rightType, shared_ptr<Type> type)
		: mTokenKind(tokenKind), mKind(kind), mLeftType(leftType), mRightType(rightType), mType(type)
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


	shared_ptr<BoundBinaryOperator> BoundBinaryOperator::Bind(Syntax::Token::Kind tokenKind, shared_ptr<Type> leftType, shared_ptr<Type> rightType)
	{
		for (auto op : sOperators)
			if (op->GetTokenKind() == tokenKind && op->GetLeftType()->Equals(leftType) && op->GetRightType()->Equals(rightType))
				return op;

		return nullptr;
	}


	Syntax::Token::Kind BoundBinaryOperator::GetTokenKind()
	{ return mTokenKind; }

	BoundBinaryOperator::Kind BoundBinaryOperator::GetKind()
	{ return mKind; }

	shared_ptr<Type> BoundBinaryOperator::GetLeftType()
	{ return mLeftType; }

	shared_ptr<Type> BoundBinaryOperator::GetRightType()
	{ return mRightType; }

	shared_ptr<Type> BoundBinaryOperator::GetType()
	{ return mType; }
}