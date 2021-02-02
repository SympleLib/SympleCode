#pragma once

#include <vector>

#include "SympleCode/Syntax/StatementSyntax.h"
#include "SympleCode/Syntax/ExpressionSyntax.h"
#include "SympleCode/Syntax/TypeSyntax.h"

namespace Symple::Syntax
{
	class VariableDeclarationSyntax;
	typedef std::vector<shared_ptr<VariableDeclarationSyntax>> VariableDeclarationList;

	class VariableDeclarationSyntax : public StatementSyntax
	{
	private:
		shared_ptr<TypeSyntax> mType;
		shared_ptr<Token> mEquals;
		shared_ptr<ExpressionSyntax> mInitializer;
	public:
		VariableDeclarationSyntax(shared_ptr<TypeSyntax> type, shared_ptr<Token> name, shared_ptr<Token> equals, shared_ptr<ExpressionSyntax> initializer)
			: StatementSyntax(name), mType(type), mEquals(equals), mInitializer(initializer) {}

		virtual Kind GetKind() override
		{ return VariableDeclaration; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			os << " '"; GetType()->PrintShort();
			if (GetName() != Token::Default)
				os << ' ' << GetName()->GetText();
			os.put('\'');

			if (GetInitializer())
			{
				std::string newIndent(indent);
				newIndent += GetAddIndent(last);

				os.put('\n'); GetInitializer()->Print(os, newIndent, true, "Initializer = ");
			}
		}

		virtual void PrintShort(std::ostream& os = std::cout) override
		{
			GetType()->PrintShort();
			if (GetName() != Token::Default)
				os << ' ' << GetName()->GetText();
		}

		shared_ptr<Token> GetName()
		{ return GetToken(); }

		shared_ptr<TypeSyntax> GetType()
		{ return mType; }

		shared_ptr<Token> GetEquals()
		{ return mEquals; }

		shared_ptr<ExpressionSyntax> GetInitializer()
		{ return mInitializer; }
	};
}