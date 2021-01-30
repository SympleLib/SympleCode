#include "SympleCode/Binding/BoundUnaryOperator.h"

namespace Symple::Binding
{
	// Explicit Declaration instead of using 'make_shared' because this is a private constructor
	std::vector<shared_ptr<BoundUnaryOperator>> BoundUnaryOperator::sOperators = {
		shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Plus, Positive, Type::IntType, Type::IntType)),
		shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Plus, Positive, Type::LongType, Type::LongType)),

		shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Dash, Negative, Type::IntType, Type::IntType)),
		shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Dash, Negative, Type::LongType, Type::LongType)),
	};

	BoundUnaryOperator::BoundUnaryOperator(Syntax::Token::Kind tokenKind, Kind kind, shared_ptr<Type> operandType, shared_ptr<Type> type)
		: mTokenKind(tokenKind), mKind(kind), mOperandType(operandType), mType(type)
	{}


	void BoundUnaryOperator::Print(std::ostream& os, std::string_view indent, bool last, std::string_view label)
	{
		Syntax::Node::PrintIndent(os, indent, last, label);
		os << "Bound Unary " << KindMap[GetKind()];
	}

	void BoundUnaryOperator::PrintShort(std::ostream& os)
	{
		os << '(' << KindMap[GetKind()] << ')';
	}


	shared_ptr<BoundUnaryOperator> BoundUnaryOperator::Bind(Syntax::Token::Kind tokenKind, shared_ptr<Type> operandType)
	{
		for (auto op : sOperators)
			if (op->GetTokenKind() == tokenKind && op->GetOperandType()->Equals(operandType))
				return op;

		return nullptr;
	}


	Syntax::Token::Kind BoundUnaryOperator::GetTokenKind()
	{ return mTokenKind; }

	BoundUnaryOperator::Kind BoundUnaryOperator::GetKind()
	{ return mKind; }

	shared_ptr<Type> BoundUnaryOperator::GetOperandType()
	{ return mOperandType; }

	shared_ptr<Type> BoundUnaryOperator::GetType()
	{ return mType; }
}