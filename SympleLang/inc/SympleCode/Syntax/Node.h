#pragma once

#include "SympleCode/Syntax/Token.h"

namespace Symple
{
	class Node
	{
	public: enum Kind : unsigned;
	protected:
		Kind mKind;
		std::shared_ptr<Token> mToken;
	public:
		Node(Kind, std::shared_ptr<Token>);
		Node(std::shared_ptr<Token>);

		Kind GetKind();
		std::shared_ptr<Token> GetToken();

		template <typename T>
		T* As()
		{ return dynamic_cast<T*>(this); }
	public:
		enum Kind : unsigned
		{
			Unknown,

			Expression,
			LiteralExpression,

			Last = LiteralExpression,
		};

		static constexpr char* KindMap[(int)Last + 1] = {
			"Unknown",

			"Expression",
			"LiteralExpression",
		};
	};
}