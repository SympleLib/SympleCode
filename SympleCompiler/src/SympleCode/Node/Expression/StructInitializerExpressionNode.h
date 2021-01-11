#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Expression/ExpressionNode.h"

#include "SympleCode/Analysis/Debug.h"

#include <vector>

namespace Symple
{
	class StructInitializerExpressionNode : public ExpressionNode
	{
	private:
		const Token* mName;

		const Token* mOpen;
		const std::vector<const ExpressionNode*> mExpressions;
		const Token* mClose;
	public:
		StructInitializerExpressionNode(const Token* name, const Token* open, const std::vector<const ExpressionNode*> expressions, const Token* close)
			: ExpressionNode(new TypeNode(Debug::GetStruct(name->GetLex()), MutModifiers, nullptr)),
			mName(name), mOpen(open), mExpressions(expressions), mClose(close)
		{
			if (mExpressions.size() > mType->GetType()->Cast<StructDeclarationNode>()->GetFields()->GetFields().size())
			{
				Diagnostics::ReportError(mClose, "Too Many Arguments");
				return;
			}

			for (unsigned int i = 0; i < mExpressions.size(); i++)
			{
				Diagnostics::ReportError(!mExpressions[i]->GetType()->CanImplicitlyCastTo(mType->GetType()->Cast<StructDeclarationNode>()
					->GetFields()->GetFields()[i]->GetType()),
					mOpen, "Unmatched Types : \n % s, \n % s",
					mType->GetType()->Cast<StructDeclarationNode>()
						->GetFields()->GetFields()[i]->GetType()->ToString("", i == mExpressions.size() - 1).c_str(), mExpressions[i]->GetType()->ToString().c_str());
			}
		}

		Kind GetKind() const override
		{
			return Kind::ListExpression;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Struct Initializer Expression " << mName->GetLex() << " {" << mExpressions.size() << "}";

			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			for (const ExpressionNode* expression : mExpressions)
				ss << '\n' << expression->ToString(indent + newIndent, expression == mExpressions.back());

			return ss.str();
		}

		const Token* GetName() const
		{
			return mName;
		}

		const Token* GetOpen() const
		{
			return mOpen;
		}

		const Token* GetClose() const
		{
			return mClose;
		}

		const std::vector<const ExpressionNode*> GetExpressions() const
		{
			return mExpressions;
		}
	};
}