#pragma once

#include <string>
#include <iostream>

#include "SympleCode/Memory.h"
#include "SympleCode/Symbol/Symbol.h"

namespace Symple::Symbol
{
	class TypeSymbol : public Symbol
	{
	public: enum TypeKind : unsigned;
	private:
		TypeKind mTypeKind;
		std::string mName;
		unsigned mSize;
		shared_ptr<TypeSymbol> mBase;
	public:
		TypeSymbol(TypeKind, std::string_view name, unsigned size, shared_ptr<TypeSymbol> base = nullptr);

		bool Equals(shared_ptr<TypeSymbol>);

		bool Is(TypeKind);
		template <typename... Args>
		bool Is(TypeKind kind, Args... kinds)
		{ return Is(kind) || Is(kinds...); }

		void Print(std::ostream & = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "");
		void PrintShort(std::ostream & = std::cout);

		TypeKind GetTypeKind();
		std::string_view GetName();
		unsigned GetSize();
		shared_ptr<TypeSymbol> GetBase();

		static shared_ptr<TypeSymbol> ErrorType;

		static shared_ptr<TypeSymbol> VoidType;
		static shared_ptr<TypeSymbol> ByteType;
		static shared_ptr<TypeSymbol> ShortType;
		static shared_ptr<TypeSymbol> IntType;
		static shared_ptr<TypeSymbol> LongType;

		static shared_ptr<TypeSymbol> BoolType;
		static shared_ptr<TypeSymbol> CharType;
		static shared_ptr<TypeSymbol> WCharType;

		static shared_ptr<TypeSymbol> FloatType;
		static shared_ptr<TypeSymbol> DoubleType;
		static shared_ptr<TypeSymbol> TripleType;

		static shared_ptr<TypeSymbol> VoidPointerType;
		static shared_ptr<TypeSymbol> BytePointerType;
	public:
		enum TypeKind : unsigned
		{
			Error,

			Void,
			Byte,
			Short,
			Int,
			Long,

			Bool,
			Char,
			WChar,
			
			Float,
			Double,
			Triple,

			Pointer,

			Last = Pointer,
		};

		static constexpr char* TypeKindMap[Last + 1] = {
			"Error",

			"Void",
			"Byte",
			"Short",
			"Int",
			"Long",

			"Float",
			"Double",
			"Triple",

			"Pointer",
		};
	};
}