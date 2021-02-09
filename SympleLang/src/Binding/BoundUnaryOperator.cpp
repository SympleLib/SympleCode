#include "SympleCode/Binding/BoundUnaryOperator.h"

namespace Symple::Binding
{
	// Explicit Declaration instead of using 'make_shared' because this is a private constructor
	std::vector<shared_ptr<BoundUnaryOperator>> BoundUnaryOperator::sOperators;
	shared_ptr<BoundUnaryOperator> BoundUnaryOperator::ErrorOperator = shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Unknown, Negative, Symbol::TypeSymbol::ErrorType, Symbol::TypeSymbol::ErrorType));;

	BoundUnaryOperator::BoundUnaryOperator(Syntax::Token::Kind tokenKind, Kind kind, shared_ptr<Symbol::TypeSymbol> operandType, shared_ptr<Symbol::TypeSymbol> type)
		: mTokenKind(tokenKind), mKind(kind), mOperandType(operandType), mType(type)
	{}


	void BoundUnaryOperator::Print(std::ostream& os, std::string_view indent, bool last, std::string_view label)
	{
		Syntax::Node::PrintIndent(os, indent, last, label);
		os << "BoundUnary" << KindMap[GetKind()] << " (" << Syntax::Token::KindMap[GetTokenKind()] << ')';
	}

	void BoundUnaryOperator::PrintShort(std::ostream& os)
	{
		os << '(' << KindMap[GetKind()] << ')';
	}


	shared_ptr<BoundUnaryOperator> BoundUnaryOperator::Bind(Syntax::Token::Kind tokenKind, shared_ptr<Symbol::TypeSymbol> operandType)
	{
		if (sOperators.empty())
			sOperators = {
				shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Plus, Positive, Symbol::TypeSymbol::IntType, Symbol::TypeSymbol::IntType)),
				shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Plus, Positive, Symbol::TypeSymbol::LongType, Symbol::TypeSymbol::LongType)),

				shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Plus, Positive, Symbol::TypeSymbol::FloatType, Symbol::TypeSymbol::FloatType)),
				shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Plus, Positive, Symbol::TypeSymbol::DoubleType, Symbol::TypeSymbol::DoubleType)),
				shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Plus, Positive, Symbol::TypeSymbol::TripleType, Symbol::TypeSymbol::TripleType)),


				shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Dash, Negative, Symbol::TypeSymbol::IntType, Symbol::TypeSymbol::IntType)),
				shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Dash, Negative, Symbol::TypeSymbol::LongType, Symbol::TypeSymbol::LongType)),

				shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Dash, Negative, Symbol::TypeSymbol::FloatType, Symbol::TypeSymbol::FloatType)),
				shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Dash, Negative, Symbol::TypeSymbol::DoubleType, Symbol::TypeSymbol::DoubleType)),
				shared_ptr<BoundUnaryOperator>(new BoundUnaryOperator(Syntax::Token::Dash, Negative, Symbol::TypeSymbol::TripleType, Symbol::TypeSymbol::TripleType)),
			};

		for (auto op : sOperators)
			if (op->GetTokenKind() == tokenKind && op->GetOperandType()->Equals(operandType))
				return op;

		return ErrorOperator;
	}


	Syntax::Token::Kind BoundUnaryOperator::GetTokenKind()
	{ return mTokenKind; }

	BoundUnaryOperator::Kind BoundUnaryOperator::GetKind()
	{ return mKind; }

	shared_ptr<Symbol::TypeSymbol> BoundUnaryOperator::GetOperandType()
	{ return mOperandType; }

	shared_ptr<Symbol::TypeSymbol> BoundUnaryOperator::GetType()
	{ return mType; }
}