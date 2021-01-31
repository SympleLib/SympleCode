#include "SympleCode/Binding/Type.h"

#include "SympleCode/Binding/Node.h"

namespace Symple::Binding
{
	shared_ptr<Type> Type::ErrorType = make_shared<Type>(Error, "error-type", -1);

	shared_ptr<Type> Type::VoidType  = make_shared<Type>(Void, "void", 0);
	shared_ptr<Type> Type::ByteType  = make_shared<Type>(Byte, "byte", 1);
	shared_ptr<Type> Type::ShortType = make_shared<Type>(Short, "short", 2);
	shared_ptr<Type> Type::IntType   = make_shared<Type>(Int, "int", 4);
	shared_ptr<Type> Type::LongType  = make_shared<Type>(Long, "long", 8);

	shared_ptr<Type> Type::FloatType   = make_shared<Type>(Float, "float", 8);
	shared_ptr<Type> Type::DoubleType  = make_shared<Type>(Double, "double", 8);
	shared_ptr<Type> Type::TripleType  = make_shared<Type>(Triple, "triple", 16);

	shared_ptr<Type> Type::VoidPointerType  = make_shared<Type>(Pointer, "*", 4, VoidType);
	shared_ptr<Type> Type::BytePointerType  = make_shared<Type>(Pointer, "*", 4, ByteType);

	Type::Type(Kind kind, std::string_view name, unsigned sz, shared_ptr<Type> base)
		: mKind(kind), mName(name), mSize(sz), mBase(base)
	{}


	bool Type::Equals(shared_ptr<Type> other)
	{
		if (GetBase() && other->GetBase())
			return Is(other->GetKind()) && GetBase()->Equals(other->GetBase());
		else if (GetBase() || other->GetBase())
			return false;
		else
			return Is(other->GetKind());
	}


	bool Type::Is(Kind kind)
	{ return mKind == kind; }


	void Type::Print(std::ostream & os, std::string_view indent, bool last, std::string_view label)
	{
		if (GetBase())
			GetBase()->Print(os);

		Node::PrintIndent(os, indent, last, label);
		os << KindMap[GetKind()];
		os << " Type";
	}

	void Type::PrintShort(std::ostream & os)
	{
		if (GetBase())
			GetBase()->PrintShort(os);

		if (GetKind() == Pointer)
			os.put('*');
		else
			os << KindMap[GetKind()];
	}


	Type::Kind Type::GetKind()
	{ return mKind; }

	std::string_view Type::GetName()
	{ return mName; }

	unsigned Type::GetSize()
	{ return mSize; }

	shared_ptr<Type> Type::GetBase()
	{ return mBase; }
}