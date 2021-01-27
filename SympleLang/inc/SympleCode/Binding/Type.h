#pragma once

#include "SympleCode/Memory.h"

namespace Symple::Binding
{
	class Type
	{
	public: enum Kind : unsigned;
	private:
		Kind mKind;
		unsigned mSize;
		shared_ptr<Type> mBase;
	public:
		Type(Kind, unsigned size, shared_ptr<Type> base = nullptr);

		bool Is(Kind);
		template <typename... Args>
		bool Is(Kind kind, Args... kinds)
		{ return Is(kind) || Is(kinds...); }

		Kind GetKind();
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