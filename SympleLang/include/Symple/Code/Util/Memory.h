#pragma once

#include "Symple/Code/Common.h"
#include <memory>

namespace Symple::Code
{
	template<typename T>
	using Slave = std::unique_ptr;
	template<typename T>
	using Ref = std::shared_ptr;
	template<typename T>
	using Friend = std::weak_ptr;

	template<typename T>
	inline constexpr T &&Pass(T &&obj)
	{ return static_cast<T &&>(obj); }

	template<typename T>
	inline constexpr T &&Pass(T &obj)
	{ return static_cast<T &&>(obj); }

	template<typename T>
	inline constexpr T &&Pass(const T &obj)
	{ return static_cast<T &&>(obj); }

	template<typename T, typename... Args>
	inline Slave<T> MakeSlave(Args&&... args)
	{ return std::make_unique<T>(std::forward<Args>(args)...); }

	template<typename T, typename... Args>
	inline Ref<T> MakeRef(Args&&... args)
	{ return std::make_shared<T>(std::forward<Args>(args)...); }

	template<typename To, typename From>
	inline Slave<To> Cast(Slave<From> &&ref)
	{
		To *cast = dynamic_cast<To *>(ref.get());
		if (cast)
		{
			Slave<To> result(cast);
			ref.release();
			return result;
		}
		else
			return Friend<To>();
	}

	template<typename To, typename From>
	inline Ref<To> Cast(Ref<From> &&ref)
	{ return std::dynamic_pointer_cast<To>(Pass(ref)); }

	template<typename To, typename From>
	inline Ref<To> Cast(const Ref<From> &ref)
	{ return std::dynamic_pointer_cast<To>(ref); }

	template <typename T>
	bool IsEmpty(const Friend<T> &ptr)
	{ return !ptr.owner_before(Friend<T>()) && !Friend<T>().owner_before(ptr); }
}