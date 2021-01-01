#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Node/Statement/StatementNode.h"
#include "SympleCode/Common/Node/Expression/ExpressionNode.h"
#include "SympleCode/Common/Node/Variable/VariableModifiersNode.h"

namespace Symple
{
	class VariableDeclarationNode : public StatementNode, public Variable
	{
	protected:
		const Token* mName;
		const Type* mType;
		const VariableModifiersNode* mModifiers;
		const ExpressionNode* mInitializer;
		const VariableDeclarationNode* mNext;
	public:
		VariableDeclarationNode(const Token* name, const Type* type, const VariableModifiersNode* modifiers, const ExpressionNode* initializer, const VariableDeclarationNode* next)
			: mName(name), mType(type), mInitializer(initializer), mModifiers(modifiers), mNext(next) {}

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
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";

			ss << '\n' << mModifiers->ToString(indent + newIndent, false);
			if (mInitializer->GetKind() != Kind::Expression)
				ss << '\n' << mInitializer->ToString(indent + newIndent, false);
			if (mNext)
				ss << '\n' << mNext->ToString(indent + newIndent);

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

		const VariableModifiersNode* GetModifiers() const
		{
			return mModifiers;
		}

		const ExpressionNode* GetInitializer() const
		{
			return mInitializer;
		}

		const VariableDeclarationNode* GetNext() const
		{
			return mNext;
		}
	};
}