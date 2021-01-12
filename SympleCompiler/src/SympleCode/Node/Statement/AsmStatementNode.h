#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Statement/StatementNode.h"

namespace Symple
{
	class AsmStatementNode : public StatementNode
	{
	private:
		const Token* mInstructions;
	public:
		AsmStatementNode(const Token* instructions)
			: mInstructions(instructions) {}

		Kind GetKind() const
		{
			return Kind::AsmStatement;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Assembly Statement";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mInstructions;
			return ss.str();
		}

		const Token* GetInstructions() const
		{
			return mInstructions;
		}
	};
}