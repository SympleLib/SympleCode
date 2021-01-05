#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Type.h"
#include "SympleCode/Node/Statement/StatementNode.h"
#include "SympleCode/Node/Expression/ExpressionNode.h"
#include "SympleCode/Node/Variable/VariableModifiersNode.h"

#include "SympleCode/Analysis/Diagnostics.h"

namespace Symple
{
	class VariableDeclarationNode : public StatementNode, public Variable
	{
	protected:
		const Token* mName;
		const TypeNode* mType;
		const VariableModifiersNode* mModifiers;
		const ExpressionNode* mInitializer;
		const VariableDeclarationNode* mNext;
	public:
		VariableDeclarationNode(const Token* name, const TypeNode* type, const VariableModifiersNode* modifiers, const ExpressionNode* initializer, const VariableDeclarationNode* next)
			: mName(name), mType(type), mInitializer(initializer), mModifiers(modifiers), mNext(next)
		{
			if (mInitializer && !mInitializer->GetType()->CanImplicitlyCastTo(mType))
				Diagnostics::ReportError(name, "Unmatched Types");
		}

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
			ss << "Variable Declaration (" << mType->GetType()->GetName() << ") " << mName->GetLex();
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

		const TypeNode* GetType() const
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