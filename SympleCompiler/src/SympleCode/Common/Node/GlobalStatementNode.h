#pragma once

#include "SympleCode/Common/Node/MemberNode.h"
#include "SympleCode/Common/Node/StatementNode.h"

namespace Symple
{
	class GlobalStatementNode : public MemberNode
	{
	private:
		const StatementNode* mStatement;
	public:
		GlobalStatementNode(const StatementNode* statement)
			: mStatement(statement) {}

		inline const StatementNode* GetStatement() const
		{
			return mStatement;
		}
	};
}