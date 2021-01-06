#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Type.h"
#include "SympleCode/Node/Member/MemberNode.h"
#include "SympleCode/Node/Expression/ExpressionNode.h"
#include "SympleCode/Node/FieldListNode.h"

#include "SympleCode/Analysis/Diagnostics.h"

namespace Symple
{
	class StructDeclarationNode : public MemberNode
	{
	private:
		const Token* mName;

		const FieldListNode* mFields;
	public:
		StructDeclarationNode(const Token* name, const FieldListNode* fields)
			: mName(name), mFields(fields) {}

		Kind GetKind() const
		{
			return Kind::StructDeclaration;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Struct Declaration (" << mName << ") " << mName->GetLex();
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";

			return ss.str();
		}

		const Token* GetName() const
		{
			return mName;
		}

		const FieldListNode* GetFields() const
		{
			return mFields;
		}
	};
}