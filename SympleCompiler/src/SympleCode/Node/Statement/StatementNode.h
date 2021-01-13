#pragma once

#include "SympleCode/Node/Node.h"

namespace Symple
{
	class StatementNode : public Node
	{
	public:
		virtual Kind GetKind() const override
		{
			return Kind::Statement;
		}

		virtual bool IsReturn() const
		{
			return false;
		}
	};
}