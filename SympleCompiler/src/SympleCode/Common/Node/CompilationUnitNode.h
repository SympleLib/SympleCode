#pragma once

#include <vector>

#include "SympleCode/Common/Node/MemberNode.h"

namespace Symple
{
	class CompilationUnitNode : public Node
	{
	private:
		const std::vector<const MemberNode*> mMembers;
	public:
		CompilationUnitNode(const std::vector<const MemberNode*>& members)
			: mMembers(members) {}

		Kind GetKind() const override
		{
			return Kind::CompilationUnit;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Compilation Unit";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			for (const MemberNode* member : mMembers)
				ss << '\n' << member->ToString(indent + newIndent, member == mMembers.back());

			return ss.str();
		}

		const std::vector<const MemberNode*>& GetMembers() const
		{
			return mMembers;
		}
	};
}