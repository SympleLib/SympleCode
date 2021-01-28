#include "SympleCode/Binding/Type.h"

namespace Symple::Binding
{
	shared_ptr<Type> Type::ErrorType = make_shared<Type>(Error, "error-type", -1);

	Type::Type(Kind kind, std::string_view name, unsigned sz, shared_ptr<Type> base)
		: mKind(kind), mName(name), mSize(sz), mBase(base)
	{}

	bool Type::Is(Kind kind)
	{ return mKind == kind; }

	Type::Kind Type::GetKind()
	{ return mKind; }

	std::string_view Type::GetName()
	{ return mName; }

	unsigned Type::GetSize()
	{ return mSize; }

	shared_ptr<Type> Type::GetBase()
	{ return mBase; }
}