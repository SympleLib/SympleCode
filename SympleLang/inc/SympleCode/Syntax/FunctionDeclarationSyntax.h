#pragma once

#include "SympleCode/Syntax/MemberSyntax.h"
#include "SympleCode/Syntax/StatementSyntax.h"
#include "SympleCode/Syntax/TypeReferenceSyntax.h"

namespace Symple::Syntax
{
	class FunctionDeclarationSyntax : public MemberSyntax
	{
	private:
		shared_ptr<TypeReferenceSyntax> mType;
		shared_ptr<StatementSyntax> mBody;
	public:
		FunctionDeclarationSyntax(shared_ptr<TypeReferenceSyntax> type, shared_ptr<Token> name, shared_ptr<StatementSyntax> body)
			: MemberSyntax(name), mType(type), mBody(body) {}

		virtual Kind GetKind() override
		{ return FunctionDeclaration; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			os << " '"; GetType()->PrintShort(os); os.put(' '); GetName()->PrintShort(os); os << "()'";

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);
		}

		shared_ptr<TypeReferenceSyntax> GetType()
		{ return mType; }

		shared_ptr<Token> GetName()
		{ return GetToken(); }

		shared_ptr<StatementSyntax> GetBody()
		{ return mBody; }
	};
}