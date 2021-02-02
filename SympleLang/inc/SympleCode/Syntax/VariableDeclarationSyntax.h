#pragma once

#include "SympleCode/Syntax/StatementSyntax.h"
#include "SympleCode/Syntax/ExpressionSyntax.h"
#include "SympleCode/Syntax/TypeReferenceSyntax.h"

namespace Symple::Syntax
{
	class VariableDeclarationSyntax: public StatementSyntax
	{
	private:
		shared_ptr<TypeReferenceSyntax> mType;
		shared_ptr<Token> mEquals;
		shared_ptr<ExpressionSyntax> mInitializer;
	public:
		VariableDeclarationSyntax(shared_ptr<TypeReferenceSyntax> type, shared_ptr<Token> name, shared_ptr<Token> equals, shared_ptr<ExpressionSyntax> initializer)
			: StatementSyntax(name), mType(type), mEquals(equals), mInitializer(initializer) {}

		virtual Kind GetKind() override
		{ return VariableDeclaration; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			os.put(' '); GetType()->PrintShort(); os.put(' '); os << GetName()->GetText();

			if (GetInitializer())
			{
				std::string newIndent(indent);
				newIndent += GetAddIndent(last);

				os.put('\n'); GetInitializer()->Print(os, newIndent, true, "Initializer = ");
			}
		}

		shared_ptr<Token> GetName()
		{ return GetToken(); }

		shared_ptr<TypeReferenceSyntax> GetType()
		{ return mType; }

		shared_ptr<Token> GetEquals()
		{ return mEquals; }

		shared_ptr<ExpressionSyntax> GetInitializer()
		{ return mInitializer; }
	};
}