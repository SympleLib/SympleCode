#pragma once

#include "SympleCode/Node/Expression/VariableAddressExpressionNode.h"

namespace Symple
{
	class DereferencePointerExpressionNode : public ModifiableExpressionNode
	{
	private:
		const Token* mSymbol;
		const ExpressionNode* mAddress;
	public:
		DereferencePointerExpressionNode(const Token* symbol, const ExpressionNode* address)
			: ModifiableExpressionNode(new TypeNode(address->GetType()->GetType(), EmptyModifiers,
				address->GetType()->GetContinue())), mSymbol(symbol), mAddress(address) {}

		Kind GetKind() const override
		{
			return Kind::DereferencePointerExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Dereference Pointer Expression";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mAddress->ToString(indent + newIndent);
			return ss.str();
		}

		const Token* GetSymbol() const
		{
			return mSymbol;
		}

		const ExpressionNode* GetAddress() const
		{
			return mAddress;
		}

		bool IsMutable() const
		{
			return mAddress->GetType()->GetModifiers()->IsMutable();
		}
	};
}