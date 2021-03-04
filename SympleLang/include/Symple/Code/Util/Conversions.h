#pragma once

#include <vector>

#include "Symple/Code/Common.h"
#include "Symple/Code/Util/Memory.h"

namespace Symple::Code
{
	#pragma region Weak Conversions
	template<typename T>
	inline std::vector<WeakRef<T>> ToWeak(const std::vector<GlobalRef<T>> &list)
	{
		std::vector<WeakRef<T>> constList(list.size());
		for (uint32 i = 0; i < list.size(); i++)
			constList[i] = list[i];
		return constList;
	}

	template<typename T>
	inline std::vector<WeakRef<T>> ToWeak(std::vector<GlobalRef<T>> &&list)
	{
		std::vector<WeakRef<T>> constList(list.size());
		for (uint32 i = 0; i < list.size(); i++)
			constList[i] = std::move(list[i]);
		return constList;
	}

	template<typename T>
	inline std::vector<WeakRef<const T>> ToWeak(const std::vector<GlobalRef<const T>> &list)
	{
		std::vector<WeakRef<const T>> constList(list.size());
		for (uint32 i = 0; i < list.size(); i++)
			constList[i] = list[i];
		return constList;
	}

	template<typename T>
	inline std::vector<WeakRef<const T>> ToWeak(std::vector<GlobalRef<const T>> &&list)
	{
		std::vector<WeakRef<const T>> constList(list.size());
		for (uint32 i = 0; i < list.size(); i++)
			constList[i] = std::move(list[i]);
		return constList;
	}
	#pragma endregion

	#pragma region Const Conversions
	template<typename T>
	inline std::vector<GlobalRef<const T>> ToConst(const std::vector<GlobalRef<T>> &list)
	{
		std::vector<GlobalRef<const T>> constList(list.size());
		for (uint32 i = 0; i < list.size(); i++)
			constList[i] = list[i];
		return constList;
	}

	template<typename T>
	inline std::vector<GlobalRef<const T>> ToConst(std::vector<GlobalRef<T>> &&list)
	{
		std::vector<GlobalRef<const T>> constList(list.size());
		for (uint32 i = 0; i < list.size(); i++)
			constList[i] = std::move(list[i]);
		return constList;
	}

	template<typename T>
	inline std::vector<WeakRef<const T>> ToConst(const std::vector<WeakRef<T>> &list)
	{
		std::vector<WeakRef<const T>> constList(list.size());
		for (uint32 i = 0; i < list.size(); i++)
			constList[i] = list[i];
		return constList;
	}

	template<typename T>
	inline std::vector<WeakRef<const T>> ToConst(std::vector<WeakRef<T>> &&list)
	{
		std::vector<WeakRef<const T>> constList(list.size());
		for (uint32 i = 0; i < list.size(); i++)
			constList[i] = std::move(list[i]);
		return constList;
	}

	template<typename T>
	inline std::vector<Scope<const T>> ToConst(std::vector<Scope<T>> &&list)
	{
		std::vector<Scope<const T>> constList(list.size());
		for (uint32 i = 0; i < list.size(); i++)
			constList[i] = std::move(list[i]);
		return constList;
	}
	#pragma endregion
}