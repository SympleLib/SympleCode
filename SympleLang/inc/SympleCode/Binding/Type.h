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

		bool Equals(shared_ptr<Type>);

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

		static shared_ptr<Type> VoidType;
		static shared_ptr<Type> ByteType;
		static shared_ptr<Type> ShortType;
		static shared_ptr<Type> IntType;
		static shared_ptr<Type> LongType;

		static shared_ptr<Type> BoolType;
		static shared_ptr<Type> CharType;
		static shared_ptr<Type> WCharType;

		static shared_ptr<Type> FloatType;
		static shared_ptr<Type> DoubleType;
		static shared_ptr<Type> TripleType;

		static shared_ptr<Type> VoidPointerType;
		static shared_ptr<Type> BytePointerType;
	public:
		enum Kind : unsigned
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

		static constexpr char* KindMap[Last + 1] = {
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