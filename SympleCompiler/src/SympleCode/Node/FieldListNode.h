#pragma once

#include "SympleCode/Common/Type.h"
#include "SympleCode/Common/Token.h"
#include "SympleCode/Node/Node.h"
#include "SympleCode/Node/Statement/VariableDeclarationNode.h"

namespace Symple
{
	class FieldListNode : public Node
	{
	private:
		const std::vector<const VariableDeclarationNode*> mFields;
		unsigned int mSize;
	public:
		FieldListNode(const std::vector<const VariableDeclarationNode*>& fields)
			: mFields(fields), mSize()
		{
			for (const VariableDeclarationNode* field : mFields)
				mSize += field->GetType()->GetSize();
		}

		Kind GetKind() const override
		{
			return Kind::FieldList;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Field List";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";

			for (const VariableDeclarationNode* field : mFields)
				ss << '\n' << field->ToString(indent + newIndent, field == mFields.back());

			return ss.str();
		}

		const std::vector<const VariableDeclarationNode*>& GetFields() const
		{
			return mFields;
		}

		unsigned int GetSize() const
		{
			return mSize;
		}
	};
}