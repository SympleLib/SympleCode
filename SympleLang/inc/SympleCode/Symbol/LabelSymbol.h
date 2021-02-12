#pragma once

#include "SympleCode/Symbol/Symbol.h"

namespace Symple::Symbol
{
	class LabelSymbol : public Symbol
	{
	private:
		std::string mLabel;
	public:
		LabelSymbol(std::string_view label)
			: mLabel(label)
		{}

		virtual Kind GetKind() override
		{ return Label; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
			os << " '" << GetLabel() << '\'';
		}

		void PrintShort(std::ostream& os = std::cout)
		{ os << GetLabel(); }

		std::string_view GetLabel()
		{ return mLabel; }
	};

	typedef std::vector<shared_ptr<LabelSymbol>> LabelList;
}