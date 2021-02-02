#pragma once

#include "SympleCode/Syntax/MemberSyntax.h"
#include "SympleCode/Syntax/StatementSyntax.h"
#include "SympleCode/Syntax/TypeSyntax.h"

namespace Symple::Syntax
{
	class FunctionDeclarationSyntax : public MemberSyntax
	{
	private:
		shared_ptr<TypeSyntax> mType;
		shared_ptr<StatementSyntax> mBody;
	public:
		FunctionDeclarationSyntax(shared_ptr<TypeSyntax> type, shared_ptr<Token> name, shared_ptr<StatementSyntax> body)
			: MemberSyntax(name), mType(type), mBody(body) {}

		virtual Kind GetKind() override
		{ return FunctionDeclaration; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);

			os << " '"; GetType()->PrintShort(os); os << ' ' << GetName()->GetText(); os << "()'";

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);

			os.put('\n'); GetBody()->Print(os, newIndent, true, "Body = ");
		}

		shared_ptr<TypeSyntax> GetType()
		{ return mType; }

		shared_ptr<Token> GetName()
		{ return GetToken(); }

		shared_ptr<StatementSyntax> GetBody()
		{ return mBody; }
	};
}