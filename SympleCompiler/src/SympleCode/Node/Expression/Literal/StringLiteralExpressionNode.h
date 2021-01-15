#pragma once

#include "SympleCode/Node/Expression/Literal/LiteralExpressionNode.h"

namespace Symple
{
	class StringLiteralExpressionNode : public LiteralExpressionNode
	{
	public:
		StringLiteralExpressionNode(const Token* literal)
			: LiteralExpressionNode(StringType, literal) {}

		Kind GetKind() const override
		{
			return Kind::StringLiteralExpression;
		}
		
		bool CanEvaluate() const override
		{
			return false;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "String Literal Expression (" << mLiteral->GetLex() << ")";

			return ss.str();
		}

		std::string Stringify() const
		{
			std::stringstream ss;
			for (const char c : mLiteral->GetLex())
				switch (c)
				{
				case '\n':
					ss << "\\\n";
					break;
				case '\r':
					ss << "\\\r";
					break;
				case '\t':
					ss << "\\\t";
					break;
				case '\"':
					ss << "\\\"";
					break;
				case '\\':
					ss << "\\\\";
					break;
				default:
					ss << c;
				}

			return ss.str();
		}
	};
}