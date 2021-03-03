#pragma once

#include <memory>

namespace Symple::Code
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using GlobalRef = std::shared_ptr<T>;

	template<typename T>
	using WeakRef = std::weak_ptr<T>;

	template<typename T, typename... Args>
	inline Scope<T> MakeScope(Args&&... args)
	{ return std::make_unique<T>(std::forward<Args>(args)...); }
	
	template<typename T, typename... Args>
	inline GlobalRef<T> MakeRef(Args&&... args)
	{ return std::make_shared<T>(std::forward<Args>(args)...); }

	template<typename To, typename From>
	inline Scope<To> Cast(Scope<From> &&ref)
	{
		To *cast = dynamic_cast<To *>(ref.get());
		if (cast)
		{
			Scope<To> result(cast);
			ref.release();
			return result;
		}
		else
			return Scope<To>();
	}

	template<typename To, typename From>
	inline GlobalRef<To> Cast(GlobalRef<From>&& ref)
	{ return std::dynamic_pointer_cast<To>(std::move(ref)); }

	template<typename To, typename From>
	inline GlobalRef<To> Cast(const GlobalRef<From>& ref)
	{ return std::dynamic_pointer_cast<To>(ref); }
}