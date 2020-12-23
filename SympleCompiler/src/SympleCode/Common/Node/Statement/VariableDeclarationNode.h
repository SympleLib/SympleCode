#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Analysis/Type.h"
#include "SympleCode/Common/Node/Statement/StatementNode.h"
#include "SympleCode/Common/Node/Expression/ExpressionNode.h"

namespace Symple
{
	class VariableDeclarationNode : public StatementNode
	{
	protected:
		const Token* mName;
		const Type* mType;
		const ExpressionNode* mInitializer;
	public:
		VariableDeclarationNode(const Token* name, const Type* type, const ExpressionNode* initializer)
			: mName(name), mType(type), mInitializer(initializer) {}

		Kind GetKind() const
		{
			return Kind::VariableDeclaration;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Variable Declaration (" << mType->GetName() << ") " << mName->GetLex();
			if (mInitializer->GetKind() != Kind::Expression)
				ss << '\n' << mInitializer->ToString(indent + (last ? " \t" : "|\t"));

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

		const ExpressionNode* GetInitializer() const
		{
			return mInitializer;
		}
	};
}