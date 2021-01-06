#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Type.h"
#include "SympleCode/Node/Member/MemberNode.h"
#include "SympleCode/Node/Expression/ExpressionNode.h"
#include "SympleCode/Node/Variable/VariableModifiersNode.h"
#include "SympleCode/Node/Variable/VariableDeclaration.h"

#include "SympleCode/Analysis/Diagnostics.h"

namespace Symple
{
	class GlobalVariableDeclarationNode : public MemberNode, public VariableDeclaration
	{
	protected:
		const Token* mName;
		const TypeNode* mType;
		const VariableModifiersNode* mModifiers;
		const ExpressionNode* mInitializer;
		const GlobalVariableDeclarationNode* mNext;
	public:
		GlobalVariableDeclarationNode(const Token* name, const TypeNode* type, const VariableModifiersNode* modifiers, const ExpressionNode* initializer, const GlobalVariableDeclarationNode* next)
			: mName(name), mType(type), mInitializer(initializer), mModifiers(modifiers), mNext(next)
		{
			if (mInitializer)
				Diagnostics::ReportError(!mInitializer->GetType()->CanImplicitlyCastTo(mType), name, "Unmatched Types:\n%s,\n%s",
					mType->ToString("", false).c_str(), mInitializer->GetType()->ToString().c_str());
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
			ss << "Global Variable Declaration (" << mType->GetType()->GetName() << ") " << mName->GetLex();
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";

			ss << '\n' << mModifiers->ToString(indent + newIndent, !(mInitializer && mInitializer->GetKind() != Kind::Expression));
			if (mInitializer && mInitializer->GetKind() != Kind::Expression)
				ss << '\n' << mInitializer->ToString(indent + newIndent, !mNext);
			if (mNext)
				ss << '\n' << mNext->ToString(indent + newIndent);

			return ss.str();
		}

		const Token* GetName() const override
		{
			return mName;
		}

		const TypeNode* GetType() const override
		{
			return mType;
		}

		const VariableModifiersNode* GetModifiers() const override
		{
			return mModifiers;
		}

		const ExpressionNode* GetInitializer() const override
		{
			return mInitializer;
		}

		const GlobalVariableDeclarationNode* GetNext() const
		{
			return mNext;
		}
	};
}