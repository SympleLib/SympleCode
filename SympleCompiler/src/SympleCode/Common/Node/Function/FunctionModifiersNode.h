#pragma once

#include "SympleCode/Common/Analysis/Type.h"
#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Node.h"
#include "SympleCode/Common/Node/Function/Function.h"

namespace Symple
{
	class FunctionModifiersNode : public Node, public Function
	{
	private:
		const std::vector<const Token*> mModifiers;
		const Token* mFormatType;
		bool mIsStatic;
	public:
		FunctionModifiersNode(const std::vector<const Token*>& modifiers)
			: mModifiers(modifiers), mFormatType(nullptr), mIsStatic(false)
		{
			for (const Token* modifier : mModifiers)
			{
				switch (modifier->GetKind())
				{
				case Token::Kind::SympleCall:
				case Token::Kind::StdCall:
				case Token::Kind::CCall:
					mFormatType = modifier;
					break;
				case Token::Kind::Static:
					mIsStatic = true;
					break;
				}
			}
		}

		Kind GetKind() const override
		{
			return Kind::FunctionModifiers;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Function Modifiers";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			for (const Token* modifier : mModifiers)
				ss << '\n' << indent + newIndent << (modifier == mModifiers.back() ? "L--\t" : "|--\t") << modifier->GetLex();

			return ss.str();
		}

		const std::vector<const Token*>& GetModifiers() const
		{
			return mModifiers;
		}

		const Token* GetFormatType() const
		{
			return mFormatType;
		}

		bool IsStatic() const
		{
			return mIsStatic;
		}
	};
}