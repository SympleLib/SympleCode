#pragma once

#include "SympleCode/Syntax/StatementSyntax.h"
#include "SympleCode/Syntax/ExpressionSyntax.h"

namespace Symple::Syntax
{
	class ReturnStatementSyntax : public StatementSyntax
	{
	private:
		shared_ptr<ExpressionSyntax> mValue;
	public:
		ReturnStatementSyntax(shared_ptr<Token> tok, shared_ptr<ExpressionSyntax> val)
			: StatementSyntax(tok), mValue(val) {}

		virtual Kind GetKind() override
		{ return ReturnStatement; }

		virtual void Print(std::ostream & os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			if (GetValue())
			{
				std::string newIndent(indent);
				newIndent += GetAddIndent(last);

				os.put('\n'); GetValue()->Print(os, newIndent, true, "Value = ");
			}
		}

		shared_ptr<ExpressionSyntax> GetValue()
		{ return mValue; }
	};
}