#include <pch.hpp>
#include "SympleCode/VariebleKeeper.hpp"

#include <unordered_map>

#include "SympleCode/Stack.hpp"

namespace Symple::VariebleKeeper
{
	std::unordered_map<char*, uint64_t> sNames;
	std::unordered_map<uint64_t, Type> sTypes;
	std::unordered_map<uint64_t, uint64_t> sVariebles;
	uint64_t sScope;

	void BeginScope()
	{
		sScope++;
	}

	void EndScope()
	{
		while (sVariebles.find(sScope) != sVariebles.end())
		{
			uint64_t ptr = sVariebles[sScope];
			char* toDel = nullptr;
			for (const auto& pair : sNames)
				if (pair.second == ptr)
				{
					toDel = pair.first;
					break;
				}
			assert(toDel);
			sNames.erase(sNames.find(toDel));

			Stack::Free(sTypes[ptr].Size);
			sVariebles.erase(sVariebles.find(sScope));
		}
		sScope--;
	}

	uint64_t Scope(char* name, const Type& ty)
	{
		assert(sNames.find(name) == sNames.end()); // Varieble already exists
		uint64_t ptr = Stack::Alloc(ty.Size);
		sNames[name] = ptr;
		sTypes[ptr] = ty;
		sVariebles[sScope] = ptr;
		return ptr;
	}

	uint64_t Get(char* name)
	{
		assert(sNames.find(name) != sNames.end()); // Varieble doesn't exist
		return sNames[name];
	}
}