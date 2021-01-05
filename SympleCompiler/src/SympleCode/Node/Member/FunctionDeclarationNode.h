#pragma once

#include "SympleCode/Node/Member/MemberNode.h"
#include "SympleCode/Node/Function/FunctionArgumentsNode.h"
#include "SympleCode/Node/Function/FunctionModifiersNode.h"
#include "SympleCode/Node/Statement/BlockStatementNode.h"

namespace Symple
{
	class FunctionDeclarationNode : public MemberNode, public Function
	{
	protected:
		const TypeNode* mType;
		const Token* mName;
		const FunctionArgumentsNode* mArguments;
		const FunctionModifiersNode* mModifiers;
		const BlockStatementNode* mBody;

		std::string mAsmName;
	public:
		FunctionDeclarationNode(const TypeNode* type, const Token* name, const FunctionArgumentsNode* arguments, const FunctionModifiersNode* modifiers, const BlockStatementNode* body)
			: mType(type), mName(name), mArguments(arguments), mModifiers(modifiers), mBody(body)
		{
			std::stringstream ss;

			std::stringstream postfix;

			if (mModifiers->GetFormatType())
				switch (mModifiers->GetFormatType()->GetModifier()->GetKind())
				{
				case Token::Kind::StdCall:
					postfix << '@' << mArguments->GetSize();
					break;
				case Token::Kind::CCall:
					break;
				case Token::Kind::SympleCall:
				SympleCall:
					postfix << '@' << mArguments->GetArguments().size() << '.';
					for (const FunctionArgumentNode* argument : mArguments->GetArguments())
						postfix << '.' << argument->GetType()->GetType()->GetSize();
					break;
				}
			else if (mName->GetLex() == "main");
			else
				goto SympleCall;
			ss << '_' << mName->GetLex() << postfix.str();

			mAsmName = ss.str();
		}

		virtual Kind GetKind() const override
		{
			return Kind::FunctionDeclaration;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Function Declaration: " << mType->GetType()->GetName() << ' ' << mName->GetLex() << "()";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			ss << '\n' << mArguments->ToString(indent + newIndent, false);
			ss << '\n' << mModifiers->ToString(indent + newIndent, false);
			ss << '\n' << mBody->ToString(indent + newIndent);

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

		const FunctionArgumentsNode* GetArguments() const
		{
			return mArguments;
		}

		const FunctionModifiersNode* GetModifiers() const
		{
			return mModifiers;
		}

		const BlockStatementNode* GetBody() const
		{
			return mBody;
		}

		const std::string& GetAsmName() const
		{
			return mAsmName;
		}
	};
}