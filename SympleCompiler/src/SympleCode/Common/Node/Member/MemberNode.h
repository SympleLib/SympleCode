#pragma once

#include "SympleCode/Common/Node/Node.h"

namespace Symple
{
	class MemberNode : public Node
	{
	public:
		virtual Kind GetKind() const override
		{
			return Kind::Member;
		}
	};
}