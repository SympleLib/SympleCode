#pragma once

#include "SympleCode/Syntax/StatementSyntax.h"

namespace Symple::Syntax
{
	class LabelSyntax : public StatementSyntax
	{
	private:
		shared_ptr<Token> mColon;
	public:
		LabelSyntax(shared_ptr<Token> label, shared_ptr<Token> colon)
			: StatementSyntax(label), mColon(colon) {}

		virtual Kind GetKind() override
		{ return Label; }

		shared_ptr<Token> GetLabel()
		{ return GetToken(); }

		shared_ptr<Token> GetColon()
		{ return mColon; }
	};
}