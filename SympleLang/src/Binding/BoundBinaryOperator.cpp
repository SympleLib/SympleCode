#include "SympleCode/Binding/BoundBinaryOperator.h"

namespace Symple::Binding
{
	// Explicit Declaration instead of using 'make_shared' because this is a private constructor
	std::vector<shared_ptr<BoundBinaryOperator>> BoundBinaryOperator::sOperators;
	shared_ptr<BoundBinaryOperator> BoundBinaryOperator::ErrorOperator;

	BoundBinaryOperator::BoundBinaryOperator(Syntax::Token::Kind tokenKind, Kind kind, shared_ptr<Type> leftType, shared_ptr<Type> rightType, shared_ptr<Type> type)
		: mTokenKind(tokenKind), mKind(kind), mLeftType(leftType), mRightType(rightType), mType(type)
	{}


	void BoundBinaryOperator::Print(std::ostream& os, std::string_view indent, bool last, std::string_view label)
	{
		Syntax::Node::PrintIndent(os, indent, last, label);
		os << "Bound Binary " << KindMap[GetKind()] << " (" << Syntax::Token::KindMap[GetTokenKind()] << ')';
	}

	void BoundBinaryOperator::PrintShort(std::ostream& os)
	{ os << '(' << KindMap[GetKind()] << ')'; }


	shared_ptr<BoundBinaryOperator> BoundBinaryOperator::Bind(Syntax::Token::Kind tokenKind, shared_ptr<Type> leftType, shared_ptr<Type> rightType)
	{
		if (sOperators.empty())
		{
			sOperators = {
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Plus, Addition, Type::IntType, Type::IntType, Type::IntType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Plus, Addition, Type::LongType, Type::LongType, Type::LongType)),

				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Dash, Subtraction, Type::IntType, Type::IntType, Type::IntType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Dash, Subtraction, Type::LongType, Type::LongType, Type::LongType)),

				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Asterisk, Multiplication, Type::IntType, Type::IntType, Type::IntType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Asterisk, Multiplication, Type::LongType, Type::LongType, Type::LongType)),

				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Slash, Division, Type::IntType, Type::IntType, Type::IntType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Slash, Division, Type::LongType, Type::LongType, Type::LongType)),

				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Percentage, Modulo, Type::IntType, Type::IntType, Type::IntType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Percentage, Modulo, Type::LongType, Type::LongType, Type::LongType)),
			};

			ErrorOperator = shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Unknown, Unknown, Type::ErrorType, Type::ErrorType, Type::ErrorType));
		}

		for (auto op : sOperators)
			if (op->GetTokenKind() == tokenKind && op->GetLeftType()->Equals(leftType) && op->GetRightType()->Equals(rightType))
				return op;

		return ErrorOperator;
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