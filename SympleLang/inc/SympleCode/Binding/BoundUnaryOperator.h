#pragma once

#include <vector>

#include "SympleCode/Syntax/Node.h"
#include "SympleCode/Syntax/Token.h"

#include "SympleCode/Binding/Type.h"

namespace Symple::Binding
{
	class BoundUnaryOperator
	{
	public: enum Kind : unsigned;
	private:
		Syntax::Token::Kind mTokenKind;
		Kind mKind;
		shared_ptr<Type> mOperandType, mType;

		BoundUnaryOperator(Syntax::Token::Kind, Kind, shared_ptr<Type> operandType, shared_ptr<Type> type);
		static std::vector<shared_ptr<BoundUnaryOperator>> sOperators;
	public:
		void Print(std::ostream & = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "");
		void PrintShort(std::ostream& os = std::cout);

		static shared_ptr<BoundUnaryOperator> Bind(Syntax::Token::Kind, shared_ptr<Type> operandType);
		static shared_ptr<BoundUnaryOperator> ErrorOperator;

		Syntax::Token::Kind GetTokenKind();
		Kind GetKind();
		shared_ptr<Type> GetOperandType();
		shared_ptr<Type> GetType();
	public:
		enum Kind : unsigned
		{
			Unknown,

			Positive,
			Negative,

			Last = Negative,
		};

		static constexpr char* KindMap[Last + 1] = {
			"Unknown",

			"Positive",
			"Negative",
		};
	};
}