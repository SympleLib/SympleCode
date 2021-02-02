#pragma once

#include "SympleCode/Syntax/FunctionDeclarationSyntax.h"

#include "SympleCode/Binding/BoundMember.h"
#include "SympleCode/Binding/Type.h"

namespace Symple::Binding
{
	class BoundFunction : public BoundMember
	{
	private:
		shared_ptr<Type> mType;
	public:
		BoundFunction(shared_ptr<Syntax::FunctionDeclarationSyntax> syntax, shared_ptr<Type> ty)
			: BoundMember(syntax), mType(ty)
		{}

		virtual Kind GetKind() override
		{ return Function; }

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
			os << ")'";

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);
		}

		shared_ptr<Type> GetType()
		{ return mType; }

		shared_ptr<Syntax::Token> GetName()
		{ return dynamic_pointer_cast<Syntax::FunctionDeclarationSyntax>(mSyntax)->GetName(); }

		// Temp
		Syntax::VariableDeclarationList GetParameters()
		{ return dynamic_pointer_cast<Syntax::FunctionDeclarationSyntax>(mSyntax)->GetParameters(); }
	};
}