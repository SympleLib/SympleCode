#pragma once

#include <map>

#include "SympleCode/Common/Node/Statement/VariableDeclarationNode.h"

namespace Symple
{
	class StructDeclarationNode : public StatementNode
	{
	private:
		const Token* mName;
		const Token* mOpen;
		std::map<std::string_view, const VariableDeclarationNode*> mVariables;
		const Token* mClose;

		int mSize;
	public:
		StructDeclarationNode(const Token* name, const Token* open, const std::vector<const VariableDeclarationNode*>& statements, const Token* close)
			: mName(name), mOpen(open), mClose(close), mSize(0)
		{
			for (const VariableDeclarationNode* statement : statements)
			{
				mVariables.insert({ statement->GetName()->GetLex(), statement });
				mSize += statement->GetType()->GetSize();
			}
		}

		Kind GetKind() const override
		{
			return Kind::BlockStatement;
		}

		std::string ToString(const std::string& indent = "", bool last = true) const override
		{
			std::stringstream ss;
			ss << indent;
			if (last)
				ss << "L--\t";
			else
				ss << "|--\t";
			ss << "Struct Declaration";
			const char* newIndent = " \t";
			if (!last)
				newIndent = "|\t";
			for (const auto& pair : mVariables)
				ss << '\n' << pair.second->ToString(indent + newIndent, pair.second == mVariables.rbegin()->second);

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

		const std::map<std::string_view, const VariableDeclarationNode*>& GetVariables() const
		{
			return mVariables;
		}

		const Token* GetClose() const
		{
			return mClose;
		}

		int GetSize() const
		{
			return mSize;
		}
	};
}