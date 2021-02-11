#pragma once

#include "SympleCode/Symbol/Symbol.h"
#include "SympleCode/Symbol/TypeSymbol.h"
#include "SympleCode/Symbol/ParameterSymbol.h"

namespace Symple::Symbol
{
	class FunctionSymbol : public Symbol
	{
	public: enum CallingConvention : unsigned;
	private:
		shared_ptr<TypeSymbol> mType;
		std::string mName;
		ParameterList mParameters;
		CallingConvention mCallingConvention;
		bool mForDll;
	public:
		FunctionSymbol(shared_ptr<TypeSymbol> ty, std::string_view name, ParameterList& params, CallingConvention conv, bool dll)
			: mType(ty), mName(name), mParameters(params), mCallingConvention(conv), mForDll(dll)
		{}

		virtual Kind GetKind() override
		{ return Function; }

		virtual void Print(std::ostream& os = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "") override
		{
			PrintIndent(os, indent, last, label);
			PrintName(os);
			os << " '" << GetName() << '\'';

			std::string newIndent(indent);
			newIndent += GetAddIndent(last);
			
			os.put('\n'); GetType()->Print(os, newIndent, GetParameters().empty(), "Return Type = ");
			for (auto param : GetParameters())
			{ os.put('\n'); param->Print(os, newIndent, param == GetParameters().back()); }
		}

		void PrintShort(std::ostream& os = std::cout)
		{
			GetType()->PrintShort(os); os << ' ' << GetName() << '(';
			for (auto param : GetParameters())
			{
				param->PrintShort(os);
				if (param != GetParameters().back())
					os << ", ";
			}
			os.put(')');
		}

		void PrintSignature(std::ostream& os = std::cout)
		{
			GetType()->PrintShort(os); os << ' ' << GetName() << '(';
			for (auto param : GetParameters())
			{
				param->GetType()->PrintShort(os);
				if (param != GetParameters().back())
					os << ", ";
			}
			os.put(')');
		}

		shared_ptr<TypeSymbol> GetType()
		{ return mType; }

		std::string_view GetName()
		{ return mName; }

		ParameterList GetParameters()
		{ return mParameters; }

		CallingConvention GetCallingConvention()
		{ return mCallingConvention; }

		bool IsForDll()
		{ return mForDll; }
	public:
		enum CallingConvention : unsigned
		{
			CDecl,
			StdCall,
		};
	};
}