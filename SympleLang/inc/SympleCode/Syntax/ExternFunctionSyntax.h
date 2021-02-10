#pragma once

#include <vector>

#include "SympleCode/Syntax/MemberSyntax.h"
#include "SympleCode/Syntax/StatementSyntax.h"
#include "SympleCode/Syntax/VariableDeclarationSyntax.h"
#include "SympleCode/Syntax/TypeSyntax.h"

namespace Symple::Syntax
{
	class ExternFunctionSyntax : public MemberSyntax
	{
	private:
		shared_ptr<Token> mKeyword;
		shared_ptr<TypeSyntax> mType;
		shared_ptr<Token> mOpenParenthesis;
		VariableDeclarationList mParameters;
		shared_ptr<Token> mCloseParenthesis;
		TokenList mModifiers;
	public:
		ExternFunctionSyntax(shared_ptr<Token> keyword, shared_ptr<TypeSyntax> type, shared_ptr<Token> name,
			shared_ptr<Token> openParen, VariableDeclarationList params, shared_ptr<Token> closeParen,
			TokenList mods)
			: MemberSyntax(name), mKeyword(keyword), mType(type), mOpenParenthesis(openParen), mParameters(std::move(params)), mCloseParenthesis(closeParen), mModifiers(mods) {}

		virtual Kind GetKind() override
		{ return ExternFunction; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			os << " '"; GetType()->PrintShort(os); os << ' ' << GetName()->GetText(); os.put('(');
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
		}

		shared_ptr<Token> GetKeyword()
		{ return mKeyword; }

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
	};
}