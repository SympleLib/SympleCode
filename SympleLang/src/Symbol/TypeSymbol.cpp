#include "SympleCode/Symbol/TypeSymbol.h"

namespace Symple::Symbol
{
	shared_ptr<TypeSymbol> TypeSymbol::ErrorType = make_shared<TypeSymbol>(Error, "error-type", -1);

	shared_ptr<TypeSymbol> TypeSymbol::VoidType  = make_shared<TypeSymbol>(Void, "void", 0);
	shared_ptr<TypeSymbol> TypeSymbol::ByteType  = make_shared<TypeSymbol>(Byte, "byte", 1);
	shared_ptr<TypeSymbol> TypeSymbol::ShortType = make_shared<TypeSymbol>(Short, "short", 2);
	shared_ptr<TypeSymbol> TypeSymbol::IntType   = make_shared<TypeSymbol>(Int, "int", 4);
	shared_ptr<TypeSymbol> TypeSymbol::LongType  = make_shared<TypeSymbol>(Long, "long", 8);

	shared_ptr<TypeSymbol> TypeSymbol::BoolType = make_shared<TypeSymbol>(Bool, "bool", 2);
	shared_ptr<TypeSymbol> TypeSymbol::CharType = make_shared<TypeSymbol>(Char, "char", 4);
	shared_ptr<TypeSymbol> TypeSymbol::WCharType = make_shared<TypeSymbol>(WChar, "wchar", 8);

	shared_ptr<TypeSymbol> TypeSymbol::FloatType   = make_shared<TypeSymbol>(Float, "float", 8, true);
	shared_ptr<TypeSymbol> TypeSymbol::DoubleType  = make_shared<TypeSymbol>(Double, "double", 8, true);
	shared_ptr<TypeSymbol> TypeSymbol::TripleType  = make_shared<TypeSymbol>(Triple, "triple", 16, true);

	shared_ptr<TypeSymbol> TypeSymbol::VoidPointerType  = make_shared<TypeSymbol>(Pointer, "*", 4, false, VoidType);
	shared_ptr<TypeSymbol> TypeSymbol::BytePointerType  = make_shared<TypeSymbol>(Pointer, "*", 4, false, ByteType);
	shared_ptr<TypeSymbol> TypeSymbol::CharPointerType  = make_shared<TypeSymbol>(Pointer, "*", 4, false, CharType);

	TypeSymbol::TypeSymbol(TypeKind kind, std::string_view name, unsigned sz, bool isFloat, shared_ptr<TypeSymbol> base)
		: mTypeKind(kind), mName(name), mSize(sz), mFloat(isFloat), mBase(base)
	{}


	bool TypeSymbol::Equals(shared_ptr<TypeSymbol> other)
	{
		if (GetBase() && other->GetBase())
			return Is(other->GetTypeKind()) && GetBase()->Equals(other->GetBase());
		else if (GetBase() || other->GetBase())
			return false;
		else
			return Is(other->GetTypeKind());
	}


	bool TypeSymbol::Is(TypeKind kind)
	{ return mTypeKind == kind; }


	void TypeSymbol::Print(std::ostream& os, std::string_view indent, bool last, std::string_view label)
	{
		PrintIndent(os, indent, last, label);
		PrintName(os);
		os << ": '";

		if (GetBase())
		{
			GetBase()->PrintShort(os);
			os.put(' ');
		}

		os << GetName() << '\'';
	}

	void TypeSymbol::PrintShort(std::ostream& os)
	{
		if (GetBase())
			GetBase()->PrintShort(os);

		os << GetName();
	}


	Symbol::Kind TypeSymbol::GetKind()
	{ return Type; }

	TypeSymbol::TypeKind TypeSymbol::GetTypeKind()
	{ return mTypeKind; }

	std::string_view TypeSymbol::GetName()
	{ return mName; }

	unsigned TypeSymbol::GetSize()
	{ return mSize; }

	bool TypeSymbol::IsFloat()
	{ return mFloat; }

	shared_ptr<TypeSymbol> TypeSymbol::GetBase()
	{ return mBase; }
}