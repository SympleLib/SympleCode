#pragma once

#include <list>

namespace Symple
{
	class Node
	{
	public:
		enum class Kind
		{
			Unknown,
			CompilationUnit,
			BinaryExpression,
		};

		static constexpr const char* KindMap[] = {
			"Unknown", "CompilationUnit", "BinaryExpression",
		};

		inline static constexpr const char* KindString(Kind kind)
		{
			return KindMap[(int)kind];
		}
	public:
		inline virtual Kind GetKind() const
		{
			return Kind::Unknown;
		}
	};
}