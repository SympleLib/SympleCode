#pragma once

#include "SympleCode/Common/Node/Node.h"

namespace Symple
{
	class CompilationUnitNode : public Node
	{
	private:
		std::list<Node*> mMembers;
	public:
		CompilationUnitNode(const std::list<Node*>& members)
			: mMembers(members) {}

		inline const std::list<Node*>& GetMembers() const
		{
			return mMembers;
		}

		inline Kind GetKind() const override
		{
			return Kind::CompilationUnit;
		}
	};
}