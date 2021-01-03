#pragma once

#include <map>
#include <string>

#include "SympleCode/Common/Token.h"
#include "SympleCode/Common/Node/Member/FunctionDeclarationNode.h"
#include "SympleCode/Common/Node/Statement/VariableDeclarationNode.h"
#include "SympleCode/Common/Node/Function/FunctionCallArgumentsNode.h"

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
		static std::vector<const Message*> sMessages;
		static std::vector<const Message*> sWarnings;
		static std::vector<const Message*> sErrors;

		static std::map<std::string_view, const FunctionDeclarationNode*> sFunctions;
		static std::map<std::string_view, const VariableDeclarationNode*> sVariables;
		static std::map<std::string_view, const VariableDeclarationNode*> pVariables;
	public:
		static void Clear();

		static void ReportError(const Token* token, const char* fmt, ...);

		static void ReportWarning(const Token* token, const char* fmt, ...);

		static void FunctionDeclaration(const FunctionDeclarationNode* function);
		static void VariableDeclaration(const VariableDeclarationNode* variable);

		static void BeginScope();
		static void EndScope();

		static const std::vector<const Message*>& GetMessages();
		static const std::vector<const Message*>& GetWarnings();
		static const std::vector<const Message*>& GetErrors();

		static const FunctionDeclarationNode* GetFunction(const std::string_view& name, const FunctionCallArgumentsNode* arguments);
		static const std::map<std::string_view, const FunctionDeclarationNode*>& GetFunctions();

		static const VariableDeclarationNode* GetVariable(const std::string_view& name);
		static const std::map<std::string_view, const VariableDeclarationNode*>& GetVariables();
	};
}