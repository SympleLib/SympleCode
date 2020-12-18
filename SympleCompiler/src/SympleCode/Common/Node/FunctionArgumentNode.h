#pragma once

#include "SympleCode/Common/Analysis/Type.h"
#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Node.h"

namespace Symple
{
	class FunctionArgumentNode : public Node
	{
	private:
		const Type* mType;
		const Token* mName;
	public:
		FunctionArgumentNode(const Type* type, const Token* name)
			: mType(type), mName(name) {}

		Kind GetKind() const override
		{
			return Kind::FunctionArgument;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Function Argument (" << mType->GetName() << ") " << mName->GetLex();

			return ss.str();
		}

		const Type* GetType() const
		{
			return mType;
		}

		const Token* GetName() const
		{
			return mName;
		}
	};
}