#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Node.h"
#include "SympleCode/Node/Function/Function.h"

#include "SympleCode/Analysis/Diagnostics.h"

namespace Symple
{
	class FunctionModifierNode : public Node, public Function
	{
	private:
		const Token* mModifier;
		bool mStatic, mFormat;
	public:
		FunctionModifierNode(const Token* modifier)
			: mModifier(modifier), mStatic(mModifier->Is(Token::Kind::Static)), mFormat(mModifier->IsEither({ Token::Kind::SympleCall, Token::Kind::CCall, Token::Kind::StdCall }))
		{
			if (!mModifier->IsEither({ Token::Kind::Static, Token::Kind::SympleCall, Token::Kind::CCall, Token::Kind::StdCall }))
				Diagnostics::ReportError(mModifier, "Illegal Modifier");
		}

		Kind GetKind() const override
		{
			return Kind::FunctionModifier;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Function Modifier [" << mModifier->GetLex() << "]";

			return ss.str();
		}

		const Token* GetModifier() const
		{
			return mModifier;
		}

		bool IsStatic() const
		{
			return mStatic;
		}

		bool IsFormat() const
		{
			return mFormat;
		}
	};
}