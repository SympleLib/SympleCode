#pragma once

#include "SympleCode/Syntax/Node.h"
#include "SympleCode/Syntax/Token.h"

#include "SympleCode/Binding/Type.h"

namespace Symple::Binding
{
	class BoundBinaryOperator
	{
	public: enum Kind : unsigned;
	private:
		Kind mKind;
		shared_ptr<Type> mLeftType, mRightType, mType;
	public:
		BoundBinaryOperator(Kind kind, shared_ptr<Type> leftType, shared_ptr<Type> rightType, shared_ptr<Type> type);

		void Print(std::ostream& = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "");

		void PrintShort(std::ostream& os = std::cout);

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