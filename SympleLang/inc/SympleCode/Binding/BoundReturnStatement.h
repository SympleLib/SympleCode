#pragma once

#include "SympleCode/Syntax/ReturnStatementSyntax.h"

#include "SympleCode/Binding/BoundStatement.h"
#include "SympleCode/Binding/BoundExpression.h"

namespace Symple::Binding
{
	class BoundReturnStatement : public BoundStatement
	{
	private:
		shared_ptr<BoundExpression> mValue;
	public:
		BoundReturnStatement(shared_ptr<Syntax::ReturnStatementSyntax> syntax, shared_ptr<BoundExpression> val)
			: BoundStatement(syntax), mValue(val) {}

		virtual Kind GetKind() override
		{ return ReturnStatement; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetValue()->Print(os, newIndent, true, "Value = ");
		}

		shared_ptr<BoundExpression> GetValue()
		{ return mValue; }
	};
}