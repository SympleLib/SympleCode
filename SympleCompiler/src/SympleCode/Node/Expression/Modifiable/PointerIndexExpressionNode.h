#pragma once

#include "SympleCode/Node/Expression/VariableAddressExpressionNode.h"

namespace Symple
{
	class PointerIndexExpressionNode : public ModifiableExpressionNode
	{
	private:
		const ExpressionNode* mAddress;
		const Token* mBracket;
		const ExpressionNode* mIndex;
	public:
		PointerIndexExpressionNode(const ExpressionNode* address, const Token* bracket, const ExpressionNode* index)
			: ModifiableExpressionNode(new TypeNode(address->GetType()->GetType(),
				address->GetType()->GetContinue() ? address->GetType()->GetContinue()->GetModifiers() : MutModifiers,
				address->GetType()->GetContinue() ? address->GetType()->GetContinue()->GetContinue() : nullptr)), mIndex(index), mBracket(bracket), mAddress(address)
		{
			Diagnostics::ReportError(address->GetType()->GetSize() != 4, mBracket, "Address Not Pointer Type");
		}

		Kind GetKind() const override
		{
			return Kind::PointerIndexExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Pointer Index Expression";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mAddress->ToString(indent + newIndent, false);
			ss << '\n' << mIndex->ToString(indent + newIndent);
			return ss.str();
		}

		const ExpressionNode* GetAddress() const
		{
			return mAddress;
		}

		const Token* GetBracket() const
		{
			return mBracket;
		}

		const ExpressionNode* GetIndex() const
		{
			return mIndex;
		}

		bool IsMutable() const
		{
			return mType->GetModifiers()->IsMutable();
		}
	};
}