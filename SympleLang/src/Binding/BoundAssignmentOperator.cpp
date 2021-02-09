#include "SympleCode/Binding/BoundAssignmentOperator.h"

#include "SympleCode/Binding/Casting.h"

namespace Symple::Binding
{
	// Explicit Declaration instead of using 'make_shared' because this is a private constructor
	std::vector<shared_ptr<BoundAssignmentOperator>> BoundAssignmentOperator::sOperators;
	shared_ptr<BoundAssignmentOperator> BoundAssignmentOperator::ErrorOperator = shared_ptr<BoundAssignmentOperator>(new BoundAssignmentOperator(Syntax::Token::Unknown, Unknown, Symbol::TypeSymbol::ErrorType, Symbol::TypeSymbol::ErrorType, Symbol::TypeSymbol::ErrorType));

	BoundAssignmentOperator::BoundAssignmentOperator(Syntax::Token::Kind tokenKind, Kind kind, shared_ptr<Symbol::TypeSymbol> leftType, shared_ptr<Symbol::TypeSymbol> rightType, shared_ptr<Symbol::TypeSymbol> type)
		: mTokenKind(tokenKind), mKind(kind), mLeftType(leftType), mRightType(rightType), mType(type)
	{}


	void BoundAssignmentOperator::Print(std::ostream& os, std::string_view indent, bool last, std::string_view label)
	{
		Syntax::Node::PrintIndent(os, indent, last, label);
		os << "BoundAssigment" << KindMap[GetKind()] << " (" << Syntax::Token::KindMap[GetTokenKind()] << ')';
	}

	void BoundAssignmentOperator::PrintShort(std::ostream& os)
	{
		os << '(' << KindMap[GetKind()] << ')';
	}


	shared_ptr<BoundAssignmentOperator> BoundAssignmentOperator::Bind(Syntax::Token::Kind tokenKind, shared_ptr<Symbol::TypeSymbol> leftType, shared_ptr<Symbol::TypeSymbol> rightType)
	{
		for (auto op : sOperators)
		{
			bool exactSame = op->GetLeftType()->Equals(leftType) && op->GetRightType()->Equals(rightType);
			bool canCast = CastTable::CanImplicitelyCast(op->GetLeftType(), leftType) && CastTable::CanImplicitelyCast(op->GetRightType(), rightType);
			if (op->GetTokenKind() == tokenKind && exactSame)
				return op;
		}

		sOperators.push_back(make_shared<BoundAssignmentOperator>(tokenKind, Assign, leftType, rightType, leftType));
		return sOperators.back();
	}


	Syntax::Token::Kind BoundAssignmentOperator::GetTokenKind()
	{
		return mTokenKind;
	}

	BoundAssignmentOperator::Kind BoundAssignmentOperator::GetKind()
	{
		return mKind;
	}

	shared_ptr<Symbol::TypeSymbol> BoundAssignmentOperator::GetLeftType()
	{
		return mLeftType;
	}

	shared_ptr<Symbol::TypeSymbol> BoundAssignmentOperator::GetRightType()
	{
		return mRightType;
	}

	shared_ptr<Symbol::TypeSymbol> BoundAssignmentOperator::GetType()
	{
		return mType;
	}
}