#pragma once

#include <string>
#include <iostream>

#include "SympleCode/Memory.h"

namespace Symple::Binding
{
	class Type
	{
	public: enum Kind : unsigned;
	private:
		Kind mKind;
		std::string mName;
		unsigned mSize;
		shared_ptr<Type> mBase;
	public:
		Type(Kind, std::string_view name, unsigned size, shared_ptr<Type> base = nullptr);

		bool Is(Kind);
		template <typename... Args>
		bool Is(Kind kind, Args... kinds)
		{ return Is(kind) || Is(kinds...); }

		void Print(std::ostream & = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "");
		void PrintShort(std::ostream & = std::cout);

		Kind GetKind();
		std::string_view GetName();
		unsigned GetSize();
		shared_ptr<Type> GetBase();

		static shared_ptr<Type> ErrorType;
	public:
		enum Kind : unsigned
		{
			Error,

			Void,
			Byte,
			Short,
			Int,
			
			Float,

			Pointer,

			Last = Pointer,
		};

		static constexpr char* KindMap[Last + 1] = {
			"Error",

			"Void",
			"Byte",
			"Short",
			"Int",

			"Float",

			"Pointer",
		};
	};
}