#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Member/FunctionDeclarationNode.h"

#include "SympleCode/Analysis/Debug.h"

namespace Symple
{
	class FunctionPointerExpressionNode : public ExpressionNode
	{
	private:
		const Token* mSymbol;
		const Token* mName;
		const FunctionArgumentsNode* mArguments;

		const FunctionDeclarationNode* mFunction;
	public:
		FunctionPointerExpressionNode(const Token* symbol, const Token* name, const FunctionArgumentsNode* arguments)
			: ExpressionNode(PtrType), mSymbol(symbol), mName(name), mArguments(arguments)
		{
			mFunction = Debug::GetFunction(mName->GetLex(), mArguments);

			Diagnostics::ReportError(!mFunction, mName, "Function Doesn't Exist");
		}

		Kind GetKind() const override
		{
			return Kind::FunctionPointerExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Function Pointer Expression " << mName->GetLex();

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mArguments->ToString(indent + newIndent);
			return ss.str();
		}

		const Token* GetSymbol() const
		{
			return mSymbol;
		}

		const Token* GetName() const
		{
			return mName;
		}

		const FunctionArgumentsNode* GetArguments() const
		{
			return mArguments;
		}

		const FunctionDeclarationNode* GetFunction() const
		{
			return mFunction;
		}
	};
}