#pragma once

#include <list>
#include <cstdio>

namespace Symple
{
	class Node
	{
	public:
		enum class Kind
		{
			Unknown,

			Expression,
			BinaryExpression,
		};

		static constexpr const char* KindMap[] = {
			"Unknown", "CompilationUnit", "BinaryExpression",
			"LiteralExpression",
		};

		inline static constexpr const char* KindString(Kind kind)
		{
			return KindMap[(int)kind];
		}

		inline virtual Kind GetKind() const
		{
			return Kind::Unknown;
		}

		inline bool Is(const Node* other) const
		{
			return InHeirarchy(other->GetKind());
		}

		inline virtual bool InHeirarchy(Kind kind) const
		{
			return false;
		}
	};
}