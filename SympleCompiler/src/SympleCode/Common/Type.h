#pragma once

#include <string>
#include <vector>

namespace Symple
{
	class Type
	{
	private:
		std::string mName;
		unsigned int mSize;
	public:
		Type(const std::string& name, unsigned int size)
			: mName(name), mSize(size) {}

		const std::string& GetName() const
		{
			return mName;
		}

		unsigned int GetSize() const
		{
			return mSize;
		}

		static const std::vector<const Type*> PrimitiveTypes;
	};
}