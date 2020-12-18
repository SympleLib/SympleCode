#pragma once

#include <map>
#include <string>

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Member/FunctionDeclarationNode.h"
#include "SympleCode/Common/Node/Statement/StructDeclarationNode.h"

#define DIAGNOSTIC_LEVEL_ERROR    2
#define DIAGNOSTIC_LEVEL_WARNING  1

namespace Symple
{
	struct Message
	{
		std::string Message;
		int Level;
		const Token* Token;
	};

	class Diagnostics
	{
	private:
		std::vector<const Message*> mMessages;
		std::vector<const Message*> mWarnings;
		std::vector<const Message*> mErrors;

		std::map<std::string_view, const FunctionDeclarationNode*> mFunctions;
		std::map<std::string_view, const StructDeclarationNode*> mStructs;
	public:
		void ReportError(const Token* token, const char* fmt, ...);

		void ReportWarning(const Token* token, const char* fmt, ...);

		void FunctionDeclaration(const FunctionDeclarationNode* function);
		void StructDeclaration(const StructDeclarationNode* ztruct);

		const std::vector<const Message*>& GetMessages() const;
		const std::vector<const Message*>& GetWarnings() const;
		const std::vector<const Message*>& GetErrors() const;

		const FunctionDeclarationNode* GetFunction(const std::string_view& name) const;
		const std::map<std::string_view, const FunctionDeclarationNode*>& GetFunctions() const;

		const StructDeclarationNode* GetStruct(const std::string_view& name) const;
		const std::map<std::string_view, const StructDeclarationNode*>& GetStructs() const;
	};
}