#pragma once

#include "SympleCode/Node/Expression/FunctionCallExpressionNode.h"

namespace Symple
{
	class FunctionPointerCallExpressionNode : public ExpressionNode, public Function
	{
	private:
		const Token* mCall;
		const Token* mName;
		const FunctionCallArgumentsNode* mArguments;
	public:
		FunctionPointerCallExpressionNode(const Token* call, const Token* name, const FunctionCallArgumentsNode* arguments)
			: ExpressionNode(Debug::GetVariable(name->GetLex())), mCall(call), mName(name), mArguments(arguments)
		{

		}

		Kind GetKind() const
		{
			return Kind::FunctionPointerCallExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Function Pointer Call Expression " << mName->GetLex() << "()";
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