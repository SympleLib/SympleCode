#pragma once

#include <vector>

#include "SympleCode/Syntax/MemberSyntax.h"
#include "SympleCode/Syntax/StatementSyntax.h"
#include "SympleCode/Syntax/VariableDeclarationSyntax.h"
#include "SympleCode/Syntax/TypeSyntax.h"

namespace Symple::Syntax
{
	class FunctionDeclarationSyntax : public MemberSyntax
	{
	private:
		shared_ptr<TypeSyntax> mType;
		shared_ptr<Token> mOpenParenthesis;
		VariableDeclarationList mParameters;
		shared_ptr<Token> mCloseParenthesis;
		TokenList mModifiers;
		shared_ptr<StatementSyntax> mBody;
	public:
		FunctionDeclarationSyntax(shared_ptr<TypeSyntax> type, shared_ptr<Token> name,
			shared_ptr<Token> openParen, VariableDeclarationList& params, shared_ptr<Token> closeParen,
			TokenList& modifiers, shared_ptr<StatementSyntax> body)
			: MemberSyntax(name), mType(type), mOpenParenthesis(openParen), mParameters(params), mCloseParenthesis(closeParen), mModifiers(modifiers), mBody(body) {}

		virtual Kind GetKind() override
		{ return FunctionDeclaration; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			os << " '"; GetType()->PrintShort(os); os << ' ' << GetName()->GetText() << '(';
			for (auto param : GetParameters())
			{
				param->GetType()->PrintShort(os);
				if (param != GetParameters().back())
					os << ", ";
			}
			os.put(')');
			if (!GetModifiers().empty())
				os.put(' ');
			for (auto mod : GetModifiers())
			{
				mod->PrintShort(os);
				if (mod != GetModifiers().back())
					os << ", ";
			}
			os.put('\'');

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			for (auto param : GetParameters()) 
			{ os.put('\n'); param->Print(os, newIndent, false, "[Param] "); }
			os.put('\n'); GetBody()->Print(os, newIndent, true, "Body = ");
		}

		shared_ptr<TypeSyntax> GetType()
		{ return mType; }

		shared_ptr<Token> GetName()
		{ return GetToken(); }

		shared_ptr<Token> GetOpenParenthesis()
		{ return mOpenParenthesis; }

		VariableDeclarationList GetParameters()
		{ return mParameters; }

		shared_ptr<Token> GetCloseParenthesis()
		{ return mCloseParenthesis; }

		TokenList& GetModifiers()
		{ return mModifiers; }

		shared_ptr<StatementSyntax> GetBody()
		{ return mBody; }
	};
}