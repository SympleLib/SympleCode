#pragma once

#include <string>
#include <vector>
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
		bool mFloat;
		

		unsigned mPointerCount;
		std::vector<char> mModifiers;
	public:
		TypeSymbol(TypeKind, std::string_view name, unsigned size, bool isFloat = false, unsigned pointerCount = 0, std::vector<char> mods = {});

		bool Equals(shared_ptr<TypeSymbol>);

		bool Is(TypeKind);
		template <typename... Args>
		bool Is(TypeKind kind, Args... kinds)
		{ return Is(kind) || Is(kinds...); }

		void Print(std::ostream & = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "");
		void PrintShort(std::ostream & = std::cout);

		virtual Kind GetKind() override;
		TypeKind GetTypeKind();
		std::string_view GetName();
		unsigned GetSize();
		bool IsFloat();

		unsigned GetPointerCount();
		std::vector<char>& GetModifiers();

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
		static shared_ptr<TypeSymbol> CharPointerType;
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

			Struct,

			Last = Struct,
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

			"Struct",
		};
	};
}