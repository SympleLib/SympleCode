#pragma once

#include "SympleCode/Common/Analysis/Type.h"

#include "SympleCode/Common/Node/MemberNode.h"
#include "SympleCode/Common/Node/BlockStatementNode.h"
#include "SympleCode/Common/Node/FunctionArgumentsNode.h"

namespace Symple
{
	class FunctionDeclarationNode : public MemberNode
	{
	protected:
		const Type* mType;
		const Token* mName;
		const FunctionArgumentsNode* mArguments;
		const BlockStatementNode* mBody;
	public:
		FunctionDeclarationNode(const Type* type, const Token* name, const FunctionArgumentsNode* arguments, const BlockStatementNode* body)
			: mType(type), mName(name),mArguments(arguments), mBody(body) {}

		virtual Kind GetKind() const override
		{
			return Kind::FunctionDeclaration;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Function Declaration: " << mType->GetName() << ' ' << mName->GetLex() << "()";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mArguments->ToString(indent + newIndent, false);
			ss << '\n' << mBody->ToString(indent + newIndent);

			return ss.str();
		}

		const Token* GetName() const
		{
			return mName;
		}

		const Type* GetType() const
		{
			return mType;
		}

		const FunctionArgumentsNode* GetArguments() const
		{
			return mArguments;
		}

		const BlockStatementNode* GetBody() const
		{
			return mBody;
		}
	};
}