#pragma once

#include "SympleCode/Node/Member/StructDeclarationNode.h"
#include "SympleCode/Node/Member/FunctionDeclarationNode.h"
#include "SympleCode/Node/Member/GlobalVariableDeclarationNode.h"

#include "SympleCode/Node/Function/FunctionCallArgumentsNode.h"

#include "SympleCode/Node/Variable/VariableDeclaration.h"

#define DIAGNOSTIC_LEVEL_ERROR    2
#define DIAGNOSTIC_LEVEL_WARNING  1

namespace Symple
{
	class Debug
	{
	private:
		static std::vector<const FunctionDeclarationNode*> sFunctions;
		static std::vector<const VariableDeclarationNode*> sVariables;
		static std::vector<unsigned int> pVariables;
		static std::vector<const GlobalVariableDeclarationNode*> sGlobalVariables;

		static std::vector<const Type*> sTypes;
		static std::vector<const StructDeclarationNode*> sStructs;
	public:
		static void Clear();

		static void FunctionDeclaration(const FunctionDeclarationNode* function);
		static void VariableDeclaration(const VariableDeclarationNode* variable);
		static void VariableDeclaration(const GlobalVariableDeclarationNode* variable);
		static void StructDeclaration(const StructDeclarationNode* type);

		static void BeginScope();
		static void EndScope();
		static unsigned int GetDepth();

		static const FunctionDeclarationNode* GetFunction(const std::string_view& name, const FunctionCallArgumentsNode* arguments);
		static const std::vector<const FunctionDeclarationNode*>& GetFunctions();

		static const Symple::VariableDeclaration* GetVariable(const std::string_view& name);
		static unsigned int GetVariableDepth(const std::string_view& name);
		static const std::vector<const VariableDeclarationNode*>& GetVariables();
		static const std::vector<const GlobalVariableDeclarationNode*>& GetGlobalVariables();

		static const Type* GetType(const std::string_view& name);
		static const std::vector<const Type*>& GetTypes();

		static const VariableDeclarationNode* GetField(const FieldListNode* list, const std::string_view& name);
		static int GetFieldIndex(const FieldListNode* list, const std::string_view& name);
		static int GetFieldOffset(const FieldListNode* list, const std::string_view& name);
		static const StructDeclarationNode* GetStruct(const std::string_view& name);
		static const std::vector<const StructDeclarationNode*>& GetStructs();
	};
}