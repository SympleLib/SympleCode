#pragma once

#include <iostream>

#include "SympleCode/Binding/Node.h"

namespace Symple::Binding
{
	class BoundConstant
	{
	public: enum Kind : unsigned;
	private:
		Kind mKind;
		char mValue[16];
	public:
		BoundConstant(Kind kind, void* data)
			: mKind(kind)
		{ __SY_ASSERT(!memcpy_s(mValue, sizeof(mValue), data, 16), "Internal error"); }

		void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "")
		{
			Node::PrintIndent(os, indent, last, label);
			os << "BoundConstant: ";

			switch (GetKind())
			{
			case Integer:
				os << GetValue<int>();
				break;
			case Float:
				os << GetValue<float>();
				break;
			}
		}

		void PrintShort(std::ostream& os = std::cout)
		{
			os << "(Constant) ";

			switch (GetKind())
			{
			case Integer:
				os << GetValue<int>();
				break;
			case Float:
				os << GetValue<float>();
				break;
			}
		}

		void SetKind(Kind kind)
		{ mKind = kind; }

		Kind GetKind()
		{ return mKind; }

		template <typename T = int>
		T& GetValue()
		{
			__SY_ASSERT(sizeof(T) <= 16, "BoundConstant buffer to large");
			return (T&)mValue;
		}
	public:
		enum Kind : unsigned
		{
			Integer,
			Float,
		};
	};
}