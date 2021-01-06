#pragma once

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Type.h"
#include "SympleCode/Node/Member/MemberNode.h"
#include "SympleCode/Node/Expression/ExpressionNode.h"
#include "SympleCode/Node/FieldListNode.h"

#include "SympleCode/Analysis/Diagnostics.h"

namespace Symple
{
	class StructDeclarationNode : public MemberNode, public Type
	{
		const FieldListNode* mFields;
	public:
		StructDeclarationNode(const Token* name, const FieldListNode* fields)
			: Type(std::string(name->GetLex()), fields->GetSize()), mFields(fields) {}

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
			ss << "Struct Declaration " << mName << " (" << mSize << " bytes) ";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";

			ss << '\n' << mFields->ToString(indent + newIndent);

			return ss.str();
		}

		const FieldListNode* GetFields() const
		{
			return mFields;
		}

		bool IsDecendent() const override
		{
			return true;
		}
	};
}