#pragma once

#include "SympleCode/Syntax/Token.h"

namespace Symple
{
	class Node
	{
	public: enum Kind;
	protected:
		Kind mKind;
		std::shared_ptr<Token> mToken;
	public:
		Node(std::shared_ptr<Token>);

		Kind GetKind();
		std::shared_ptr<Token> GetToken();

		template <typename T>
		T* As()
		{ return dynamic_cast<T*>(this); }
	public:
		enum Kind
		{
			Unknown,

			Last = Unknown,
		};

		static constexpr char* KindMap[(int)Last + 1] = {
			"Unknown"
		};
	};
}