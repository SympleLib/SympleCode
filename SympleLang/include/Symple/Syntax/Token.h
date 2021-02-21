#pragma once

#include "Symple/Common/Common.h"

namespace Symple
{
	class SY_API Token
	{
	public: enum Kind: uint8;
	private:
		std::string_view Text;
	public:
	public:
		enum Kind: uint8
		{
			EndOfFile,

			Number,
			Identifier,

			// Markings
			First = EndOfFile,
			Last = Identifier,
			Count, // Number of token kinds
		};

		static constexpr const char *const KindNames[Count] = {
			"EndOfFile",

			"Number",
			"Identifier",
		};
	};
}