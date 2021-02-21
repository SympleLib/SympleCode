#pragma once

#include <memory>

namespace Symple
{
	template<typename T>
	using Scope = std::unique_ptr<T>;

	template<typename T>
	using GlobalRef = std::shared_ptr<T>;

	template<typename T>
	using WeakRef = std::weak_ptr<T>;

	template<typename T, typename... Args>
	inline Scope<T> MakeScope(Args&&... args)
	{ return std::make_unique(std::forward(args)...); }

	template<typename T, typename... Args>
	inline GlobalRef<T> MakeGlobalRef(Args&&... args)
	{ return std::make_shared(std::forward(args)...); }
}