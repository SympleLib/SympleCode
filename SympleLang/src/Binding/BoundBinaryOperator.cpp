#include "SympleCode/Binding/BoundBinaryOperator.h"

#include "SympleCode/Binding/Casting.h"

namespace Symple::Binding
{
	// Explicit Declaration instead of using 'make_shared' because this is a private constructor
	std::vector<shared_ptr<BoundBinaryOperator>> BoundBinaryOperator::sOperators;
	shared_ptr<BoundBinaryOperator> BoundBinaryOperator::ErrorOperator;

	BoundBinaryOperator::BoundBinaryOperator(Syntax::Token::Kind tokenKind, Kind kind, shared_ptr<Symbol::TypeSymbol> leftType, shared_ptr<Symbol::TypeSymbol> rightType, shared_ptr<Symbol::TypeSymbol> type)
		: mTokenKind(tokenKind), mKind(kind), mLeftType(leftType), mRightType(rightType), mType(type)
	{}


	void BoundBinaryOperator::Print(std::ostream& os, std::string_view indent, bool last, std::string_view label)
	{
		Syntax::Node::PrintIndent(os, indent, last, label);
		os << "BoundBinary" << KindMap[GetKind()] << " (" << Syntax::Token::KindMap[GetTokenKind()] << ')';
	}

	void BoundBinaryOperator::PrintShort(std::ostream& os)
	{ os << '(' << KindMap[GetKind()] << ')'; }


	shared_ptr<BoundBinaryOperator> BoundBinaryOperator::Bind(Syntax::Token::Kind tokenKind, shared_ptr<Symbol::TypeSymbol> leftType, shared_ptr<Symbol::TypeSymbol> rightType)
	{
		if (sOperators.empty())
		{
			sOperators = {
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Plus, Addition, Symbol::TypeSymbol::IntType, Symbol::TypeSymbol::IntType, Symbol::TypeSymbol::IntType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Plus, Addition, Symbol::TypeSymbol::LongType, Symbol::TypeSymbol::LongType, Symbol::TypeSymbol::LongType)),

				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Plus, Addition, Symbol::TypeSymbol::FloatType, Symbol::TypeSymbol::FloatType, Symbol::TypeSymbol::FloatType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Plus, Addition, Symbol::TypeSymbol::DoubleType, Symbol::TypeSymbol::DoubleType, Symbol::TypeSymbol::DoubleType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Plus, Addition, Symbol::TypeSymbol::TripleType, Symbol::TypeSymbol::TripleType, Symbol::TypeSymbol::TripleType)),


				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Dash, Subtraction, Symbol::TypeSymbol::IntType, Symbol::TypeSymbol::IntType, Symbol::TypeSymbol::IntType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Dash, Subtraction, Symbol::TypeSymbol::LongType, Symbol::TypeSymbol::LongType, Symbol::TypeSymbol::LongType)),

				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Dash, Subtraction, Symbol::TypeSymbol::FloatType, Symbol::TypeSymbol::FloatType, Symbol::TypeSymbol::FloatType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Dash, Subtraction, Symbol::TypeSymbol::DoubleType, Symbol::TypeSymbol::DoubleType, Symbol::TypeSymbol::DoubleType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Dash, Subtraction, Symbol::TypeSymbol::TripleType, Symbol::TypeSymbol::TripleType, Symbol::TypeSymbol::TripleType)),


				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Asterisk, Multiplication, Symbol::TypeSymbol::IntType, Symbol::TypeSymbol::IntType, Symbol::TypeSymbol::IntType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Asterisk, Multiplication, Symbol::TypeSymbol::LongType, Symbol::TypeSymbol::LongType, Symbol::TypeSymbol::LongType)),

				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Asterisk, Multiplication, Symbol::TypeSymbol::FloatType, Symbol::TypeSymbol::FloatType, Symbol::TypeSymbol::FloatType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Asterisk, Multiplication, Symbol::TypeSymbol::DoubleType, Symbol::TypeSymbol::DoubleType, Symbol::TypeSymbol::DoubleType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Asterisk, Multiplication, Symbol::TypeSymbol::TripleType, Symbol::TypeSymbol::TripleType, Symbol::TypeSymbol::TripleType)),


				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Slash, Division, Symbol::TypeSymbol::IntType, Symbol::TypeSymbol::IntType, Symbol::TypeSymbol::IntType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Slash, Division, Symbol::TypeSymbol::LongType, Symbol::TypeSymbol::LongType, Symbol::TypeSymbol::LongType)),

				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Slash, Division, Symbol::TypeSymbol::FloatType, Symbol::TypeSymbol::FloatType, Symbol::TypeSymbol::FloatType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Slash, Division, Symbol::TypeSymbol::DoubleType, Symbol::TypeSymbol::DoubleType, Symbol::TypeSymbol::DoubleType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Slash, Division, Symbol::TypeSymbol::TripleType, Symbol::TypeSymbol::TripleType, Symbol::TypeSymbol::TripleType)),


				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Percentage, Modulo, Symbol::TypeSymbol::IntType, Symbol::TypeSymbol::IntType, Symbol::TypeSymbol::IntType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Percentage, Modulo, Symbol::TypeSymbol::LongType, Symbol::TypeSymbol::LongType, Symbol::TypeSymbol::LongType)),

				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Percentage, Modulo, Symbol::TypeSymbol::FloatType, Symbol::TypeSymbol::FloatType, Symbol::TypeSymbol::FloatType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Percentage, Modulo, Symbol::TypeSymbol::DoubleType, Symbol::TypeSymbol::DoubleType, Symbol::TypeSymbol::DoubleType)),
				shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Percentage, Modulo, Symbol::TypeSymbol::TripleType, Symbol::TypeSymbol::TripleType, Symbol::TypeSymbol::TripleType)),
			};

			ErrorOperator = shared_ptr<BoundBinaryOperator>(new BoundBinaryOperator(Syntax::Token::Unknown, Unknown, Symbol::TypeSymbol::ErrorType, Symbol::TypeSymbol::ErrorType, Symbol::TypeSymbol::ErrorType));
		}

		for (auto op : sOperators)
		{
			bool exactSame = op->GetLeftType()->Equals(leftType) && op->GetRightType()->Equals(rightType);
			bool canCast = CastTable::CanImplicitelyCast(op->GetLeftType(), leftType) && CastTable::CanImplicitelyCast(op->GetRightType(), rightType);
			if (op->GetTokenKind() == tokenKind && exactSame)
				return op;
		}

		return ErrorOperator;
	}


	Syntax::Token::Kind BoundBinaryOperator::GetTokenKind()
	{ return mTokenKind; }

	BoundBinaryOperator::Kind BoundBinaryOperator::GetKind()
	{ return mKind; }

	shared_ptr<Symbol::TypeSymbol> BoundBinaryOperator::GetLeftType()
	{ return mLeftType; }

	shared_ptr<Symbol::TypeSymbol> BoundBinaryOperator::GetRightType()
	{ return mRightType; }

	shared_ptr<Symbol::TypeSymbol> BoundBinaryOperator::GetType()
	{ return mType; }
}