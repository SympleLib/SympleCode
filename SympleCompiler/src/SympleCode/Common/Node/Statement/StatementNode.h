#pragma once

#include "SympleCode/Common/Node/Node.h"

namespace Symple
{
	class StatementNode : public Node
	{
	public:
		virtual Kind GetKind() const override
		{
			return Kind::Statement;
		}
	};
}