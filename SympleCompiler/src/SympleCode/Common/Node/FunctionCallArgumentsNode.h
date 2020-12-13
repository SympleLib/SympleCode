#pragma once

#include <vector>

#include "SympleCode/Common/Node/ExpressionNode.h"

namespace Symple
{
	class FunctionCallArgumentsNode : public Node
	{
	private:
		const Token* mOpen;
		const std::vector<const ExpressionNode*> mArguments;
		const Token* mClose;
	public:
		FunctionCallArgumentsNode(const Token* open, const std::vector<const ExpressionNode*>& arguments, const Token* close)
			: mOpen(open), mArguments(arguments), mClose(close) {}

		Kind GetKind() const override
		{
			return Kind::FunctionCallArguments;
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
			for (const ExpressionNode* argument : mArguments)
				ss << '\n' << argument->ToString(indent + newIndent, argument == mArguments.back());

			return ss.str();
		}

		const Token* GetOpen() const
		{
			return mOpen;
		}

		const std::vector<const ExpressionNode*>& GetArguments() const
		{
			return mArguments;
		}

		const Token* GetClose() const
		{
			return mClose;
		}
	};
}