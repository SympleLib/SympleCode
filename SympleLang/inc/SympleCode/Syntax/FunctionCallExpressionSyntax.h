#pragma once

#include "SympleCode/Syntax/ExpressionSyntax.h"

namespace Symple::Syntax
{
	class FunctionCallExpressionSyntax : public ExpressionSyntax
	{
	private:
		shared_ptr<Token> mOpenParenthesis;
		ExpressionList mArguments;
		shared_ptr<Token> mCloseParenthesis;
	public:
		FunctionCallExpressionSyntax(shared_ptr<Token> name, shared_ptr<Token> openParen, ExpressionList args, shared_ptr<Token> closeParen)
			: ExpressionSyntax(name) {}

		virtual Kind GetKind() override
		{ return FunctionCallExpression; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			os << " '"; GetName()->GetText(); os << "()'";

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			for (auto arg : GetArguments())
			{ os.put('\n'); arg->Print(os, newIndent, arg == GetArguments().back()); }
		}

		shared_ptr<Token> GetName()
		{ return GetToken(); }

		shared_ptr<Token> GetOpenParenthesis()
		{ return mOpenParenthesis; }

		ExpressionList GetArguments()
		{ return mArguments; }

		shared_ptr<Token> GetCloseParenthesis()
		{ return mCloseParenthesis; }
	};
}