#pragma once

#include <vector>

#include "SympleCode/Syntax/Node.h"
#include "SympleCode/Syntax/Token.h"

#include "SympleCode/Symbol/TypeSymbol.h"

namespace Symple::Binding
{
	class BoundAssignmentOperator
	{
	public: enum Kind : unsigned;
	private:
		Syntax::Token::Kind mTokenKind;
		Kind mKind;
		shared_ptr<Symbol::TypeSymbol> mLeftType, mRightType, mType;

		BoundAssignmentOperator(Syntax::Token::Kind, Kind, shared_ptr<Symbol::TypeSymbol> leftType, shared_ptr<Symbol::TypeSymbol> rightType, shared_ptr<Symbol::TypeSymbol> type);
		static std::vector<shared_ptr<BoundAssignmentOperator>> sOperators;
	public:
		void Print(std::ostream & = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "");
		void PrintShort(std::ostream& os = std::cout);

		static shared_ptr<BoundAssignmentOperator> Bind(Syntax::Token::Kind, shared_ptr<Symbol::TypeSymbol> leftType, shared_ptr<Symbol::TypeSymbol> rightType);
		static shared_ptr<BoundAssignmentOperator> ErrorOperator;

		Syntax::Token::Kind GetTokenKind();
		Kind GetKind();
		shared_ptr<Symbol::TypeSymbol> GetLeftType();
		shared_ptr<Symbol::TypeSymbol> GetRightType();
		shared_ptr<Symbol::TypeSymbol> GetType();
	public:
		enum Kind : unsigned
		{
			Unknown,

			Addition,
			Subtraction,
			Multiplication,
			Division,
			Modulo,

			Last = Modulo,
		};

		static constexpr char* KindMap[Last + 1] = {
			"Unknown",

			"Addition",
			"Subtraction",
			"Multiplication",
			"Division",
			"Modulo",
		};
	};
}