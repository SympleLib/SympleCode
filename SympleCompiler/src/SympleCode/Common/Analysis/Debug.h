#pragma once

#include "SympleCode/Common/Node/Member/FunctionDeclarationNode.h"
#include "SympleCode/Common/Node/Function/FunctionCallArgumentsNode.h"

#define DIAGNOSTIC_LEVEL_ERROR    2
#define DIAGNOSTIC_LEVEL_WARNING  1

namespace Symple
{
	class Debug
	{
	private:
		static std::vector<const FunctionDeclarationNode*> sFunctions;
		static std::vector<const VariableDeclarationNode*> sVariables;
		static std::vector<const VariableDeclarationNode*> pVariables;

		static std::vector<const Type*> sTypes;
	public:
		static void Clear();

		static void FunctionDeclaration(const FunctionDeclarationNode* function);
		static void VariableDeclaration(const VariableDeclarationNode* variable);
		static void TypeDeclaration(const Type* type);

		static void BeginScope();
		static void EndScope();

		static const FunctionDeclarationNode* GetFunction(const std::string_view& name, const FunctionCallArgumentsNode* arguments);
		static const std::vector<const FunctionDeclarationNode*>& GetFunctions();

		static const VariableDeclarationNode* GetVariable(const std::string_view& name);
		static const std::vector<const VariableDeclarationNode*>& GetVariables();

		static const Type* GetType(const std::string_view& name);
		static const std::vector<const Type*>& GetTypes();
	};
}