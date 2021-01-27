#include "SympleCode/Binding/Type.h"

namespace Symple::Binding
{
	shared_ptr<Type> Type::ErrorType = make_shared<Type>(Error, -1);

	Type::Type(Kind kind, unsigned sz, shared_ptr<Type> base)
		: mKind(kind), mSize(sz), mBase(base)
	{}

	bool Type::Is(Kind kind)
	{ return mKind == kind; }

	Type::Kind Type::GetKind()
	{ return mKind; }

	unsigned Type::GetSize()
	{ return mSize; }

	shared_ptr<Type> Type::GetBase()
	{ return mBase; }
}