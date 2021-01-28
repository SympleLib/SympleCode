#include "SympleCode/Binding/Type.h"

#include "SympleCode/Syntax/Node.h"

namespace Symple::Binding
{
	shared_ptr<Type> Type::ErrorType = make_shared<Type>(Error, "error-type", -1);

	Type::Type(Kind kind, std::string_view name, unsigned sz, shared_ptr<Type> base)
		: mKind(kind), mName(name), mSize(sz), mBase(base)
	{}

	bool Type::Is(Kind kind)
	{ return mKind == kind; }


	void Type::Print(std::ostream & os, std::string_view indent, bool last, std::string_view label)
	{
		if (GetBase())
			GetBase()->Print(os);

		Syntax::Node::PrintIndent(os, indent, last, label);
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