#pragma once

#include "SympleCode/Common/Node/MemberNode.h"
#include "SympleCode/Common/Node/BlockStatementNode.h"

namespace Symple
{
	class FunctionDeclarationNode : public MemberNode
	{
	private:
		const Token* mType;
		const Token* mName;
		const BlockStatementNode* mBody;
	public:
		FunctionDeclarationNode(const Token* type, const Token* name, const BlockStatementNode* body)
			: mType(type), mName(name), mBody(body) {}

		Kind GetKind() const override
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
			ss << "Function Declaration: " << mType->GetLex() << ' ' << mName->GetLex() << "()";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mBody->ToString(indent + newIndent);

			return ss.str();
		}

		const Token* GetName() const
		{
			return mName;
		}

		const Token* GetType() const
		{
			return mType;
		}

		const BlockStatementNode* GetBody() const
		{
			return mBody;
		}
	};
}