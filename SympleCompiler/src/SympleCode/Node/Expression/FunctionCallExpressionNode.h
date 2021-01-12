#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Analysis/Debug.h"

namespace Symple
{
	class FunctionCallExpressionNode : public ExpressionNode, public Function
	{
	private:
		const Token* mName;
		const FunctionCallArgumentsNode* mArguments;
	public:
		FunctionCallExpressionNode(const Token* name, const FunctionCallArgumentsNode* arguments)
			: ExpressionNode(Debug::GetFunction(name->GetLex(), arguments) ? Debug::GetFunction(name->GetLex(), arguments)->GetType() : ErrorType), mName(name), mArguments(arguments)
		{
			Diagnostics::ReportError(!Debug::GetFunction(name->GetLex(), arguments), name, "Function Doesn't Exist");
		}

		Kind GetKind() const
		{
			return Kind::FunctionCallExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Function Call Expression " << mName->GetLex() << "()";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mArguments->ToString(indent + newIndent);

			return ss.str();
		}

		const Token* GetName() const
		{
			return mName;
		}

		const FunctionCallArgumentsNode* GetArguments() const
		{
			return mArguments;
		}
	};
}