#pragma once

#include <list>
#include <cstdio>
#include <cassert>
#include <sstream>

#include "SympleCode/Common/Node/Node.h"

namespace Symple
{
	class Link
	{
	private:
		const Node* mNode;
	public:
		Link() = default;
		Link(const Node* node)
			: mNode(node) {}

		typedef Node::Kind Kind;

		static constexpr const char* KindString(Kind kind)
		{
			return Node::KindMap[(int)kind];
		}

		template<typename T>
		bool Is() const
		{
			return dynamic_cast<const T*>(this);
		}

		template<typename T>
		T* Cast()
		{
			return dynamic_cast<T*>(this);
		}

		template<typename T>
		const T* Cast() const
		{
			return dynamic_cast<const T*>(this);
		}

		const Node* GetNode() const
		{
			return mNode;
		}

		virtual Kind GetKind() const
		{
			return Kind::Unknown;
		}

		virtual std::string ToString(const std::string& indent = "", bool last = true) const
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Link of Kind: " << KindString(GetKind());
			return ss.str();
		}
	};
}