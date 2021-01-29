#pragma once

#include <vector>

#include "SympleCode/Syntax/Node.h"
#include "SympleCode/Syntax/Token.h"

#include "SympleCode/Binding/Type.h"

namespace Symple::Binding
{
	class BoundBinaryOperator
	{
	public: enum Kind : unsigned;
	private:
		Syntax::Token::Kind mTokenKind;
		Kind mKind;
		shared_ptr<Type> mLeftType, mRightType, mType;

		BoundBinaryOperator(Syntax::Token::Kind, Kind, shared_ptr<Type> leftType, shared_ptr<Type> rightType, shared_ptr<Type> type);
		static std::vector<shared_ptr<BoundBinaryOperator>> sOperators;
	public:
		void Print(std::ostream& = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "");
		void PrintShort(std::ostream& os = std::cout);

		shared_ptr<BoundBinaryOperator> Bind(Syntax::Token::Kind, shared_ptr<Type> leftType, shared_ptr<Type> rightType);

		Syntax::Token::Kind GetTokenKind();
		Kind GetKind();
		shared_ptr<Type> GetLeftType();
		shared_ptr<Type> GetRightType();
		shared_ptr<Type> GetType();
	public:
		enum Kind : unsigned
		{
			Addition,
			Subtraction,
			Multiplication,
			Division,

			Last = Division,
		};

		static constexpr char* KindMap[Last + 1] = {
			"Addition",
			"Subtraction",
			"Multiplication",
			"Division",
		};
	};
}