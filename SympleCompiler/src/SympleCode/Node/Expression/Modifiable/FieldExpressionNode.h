#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Expression/Modifiable/ModifiableExpressionNode.h"

#include "SympleCode/Analysis/Debug.h"

namespace Symple
{
	class FieldExpressionNode : public ModifiableExpressionNode
	{
	private:
		const ModifiableExpressionNode* mCallee;
		const Token* mName;

		unsigned int mOffset;
	public:
		FieldExpressionNode(const ModifiableExpressionNode* callee, const Token* name)
			: ModifiableExpressionNode(Debug::GetField(callee->GetType()->GetType()->Cast<StructDeclarationNode>()->GetFields(), name->GetLex())->GetType()),
			mCallee(callee), mName(name), mOffset(Debug::GetFieldOffset(callee->GetType()->GetType()->Cast<StructDeclarationNode>()->GetFields(), name->GetLex()))
		{
			Diagnostics::ReportError(Debug::GetField(callee->GetType()->GetType()->Cast<StructDeclarationNode>()->GetFields(), name->GetLex())
				->GetModifiers()->IsPrivate(), name, "Field is Private");
		}

		Kind GetKind() const override
		{
			return Kind::FieldExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Field Expression " << mName->GetLex();
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";

			ss << '\n' << mCallee->ToString(indent + newIndent);

			return ss.str();
		}

		const ModifiableExpressionNode* GetCallee() const
		{
			return mCallee;
		}

		const Token* GetName() const
		{
			return mName;
		}

		unsigned int GetOffset() const
		{
			return mOffset;
		}

		bool IsMutable() const override
		{
			return Debug::GetField(mCallee->GetType()->GetType()->Cast<StructDeclarationNode>()->GetFields(), mName->GetLex())
				->GetType()->GetModifiers()->IsMutable() && mCallee->GetType()->GetModifiers()->IsMutable();
		}
	};
}