#pragma once

#include <vector>

#include "SympleCode/Syntax/Node.h"
#include "SympleCode/Syntax/Token.h"

#include "SympleCode/Symbol/TypeSymbol.h"

namespace Symple::Binding
{
	class BoundBinaryOperator
	{
	public: enum Kind : unsigned;
	private:
		Syntax::Token::Kind mTokenKind;
		Kind mKind;
		shared_ptr<Symbol::TypeSymbol> mLeftType, mRightType, mType;
		bool mMutable;

		BoundBinaryOperator(Syntax::Token::Kind, Kind, shared_ptr<Symbol::TypeSymbol> leftType, shared_ptr<Symbol::TypeSymbol> rightType, shared_ptr<Symbol::TypeSymbol> type, bool isMutable = false);
		static std::vector<shared_ptr<BoundBinaryOperator>> sOperators;
	public:
		void Print(std::ostream& = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "");
		void PrintShort(std::ostream& os = std::cout);

		static shared_ptr<BoundBinaryOperator> Bind(Syntax::Token::Kind, shared_ptr<Symbol::TypeSymbol> leftType, shared_ptr<Symbol::TypeSymbol> rightType);
		static shared_ptr<BoundBinaryOperator> ErrorOperator;

		bool IsMutable();

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

			Assign,

			Last = Assign,
		};

		static constexpr char* KindMap[Last + 1] = {
			"Unknown",

			"Addition",
			"Subtraction",
			"Multiplication",
			"Division",
			"Modulo",

			"Assign",
		};
	};
}