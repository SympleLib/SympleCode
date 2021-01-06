#pragma once

#include <string>
#include <vector>

namespace Symple
{
	class Type
	{
	protected:
		const std::string mName;
		const unsigned int mSize;
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

		template<typename T>
		bool Is() const
		{
			return dynamic_cast<const T*>(this);
		}

		template<typename T>
		T* Cast()
		{
			return dynamic_cast<T*>(this);
		}

		template<typename T>
		const T* Cast() const
		{
			return dynamic_cast<const T*>(this);
		}

		// Mainly for use of `dynamic_cast`
		virtual bool IsDecendent() const
		{
			return false;
		}

		static const std::vector<const Type*> PrimitiveTypes;

		struct PrimitiveType
		{
			static const Type* const Void;
			static const Type* const Byte;
			static const Type* const Short;
			static const Type* const Int;

			static const Type* const Bool;
			static const Type* const Char;
			static const Type* const WChar;

			static const Type* const Error;
		};
	};
}