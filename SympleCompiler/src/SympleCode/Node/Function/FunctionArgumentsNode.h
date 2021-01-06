#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Function/FunctionArgumentNode.h"

namespace Symple
{
	class FunctionArgumentsNode : public Node, public Function
	{
	private:
		const Token* mOpen;
		const std::vector<const FunctionArgumentNode*> mArguments;
		const Token* mClose;
		unsigned int mSize;
	public:
		FunctionArgumentsNode(const Token* open, const std::vector<const FunctionArgumentNode*>& arguments, const Token* close)
			: mOpen(open), mArguments(arguments), mClose(close), mSize(0)
		{
			for (const FunctionArgumentNode* argument : mArguments)
				mSize += argument->GetType()->GetSize();
		}

		Kind GetKind() const override
		{
			return Kind::FunctionArguments;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Function Arguments";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			for (const FunctionArgumentNode* argument : mArguments)
				ss << '\n' << argument->ToString(indent + newIndent, argument == mArguments.back());

			return ss.str();
		}

		const Token* GetOpen() const
		{
			return mOpen;
		}

		const std::vector<const FunctionArgumentNode*>& GetArguments() const
		{
			return mArguments;
		}

		const Token* GetClose() const
		{
			return mClose;
		}

		unsigned int GetSize() const
		{
			return mSize;
		}
	};
}