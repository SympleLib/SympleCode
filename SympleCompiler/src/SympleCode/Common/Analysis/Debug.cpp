#include "SympleCode/Common/Analysis/Debug.h"

#include <cstdarg>
#include <iostream>

namespace Symple
{
	std::vector<const FunctionDeclarationNode*> Debug::sFunctions;
	std::vector<const VariableDeclarationNode*> Debug::sVariables;
	std::vector<const VariableDeclarationNode*> Debug::pVariables;

	std::vector<const Type*> Debug::sTypes;

	void Debug::Clear()
	{
		sFunctions.clear();
		sVariables.clear();
		pVariables.clear();

		sTypes = Type::PrimitiveTypes;
	}

	void Debug::FunctionDeclaration(const FunctionDeclarationNode* function)
	{
		sFunctions.push_back(function);
	}

	void Debug::VariableDeclaration(const VariableDeclarationNode* variable)
	{
		sVariables.push_back(variable);
	}

	void Debug::TypeDeclaration(const Type* type)
	{
		sTypes.push_back(type);
	}

	void Debug::BeginScope()
	{
		pVariables = sVariables;
	}

	void Debug::EndScope()
	{
		sVariables = pVariables;
	}

	const FunctionDeclarationNode* Debug::GetFunction(const std::string_view& name, const FunctionCallArgumentsNode* arguments)
	{
		for (const FunctionDeclarationNode* function : sFunctions)
		{
			if (function->GetName()->GetLex() == name)
			{
				if (function->GetModifiers()->GetFormatType())
					switch (function->GetModifiers()->GetFormatType()->GetModifier()->GetKind())
					{
					case Token::Kind::SympleCall:
						goto SympleCall;
					case Token::Kind::StdCall:
					case Token::Kind::CCall:
						return function;
					}

			SympleCall:
				bool same = true;

				for (unsigned int i = 0; i < arguments->GetArguments().size(); i++)
				{
					if (i >= function->GetArguments()->GetArguments().size())
					{
						same = false;

						break;
					}

					same &= arguments->GetArguments()[i]->GetType()->GetType() == function->GetArguments()->GetArguments()[i]->GetType()->GetType();
				}

				if (same)
					return function;
			}
		}

		return nullptr;
	}

	const std::vector<const FunctionDeclarationNode*>& Debug::GetFunctions()
	{
		return sFunctions;
	}

	const VariableDeclarationNode* Debug::GetVariable(const std::string_view& name)
	{
		for (const VariableDeclarationNode* variable : sVariables)
			if (variable->GetName()->GetLex() == name)
				return variable;

		return nullptr;
	}

	const std::vector<const VariableDeclarationNode*>& Debug::GetVariables()
	{
		return sVariables;
	}

	const Type* Debug::GetType(const std::string_view& name)
	{
		for (const Type* type : sTypes)
			if (type->GetName() == name)
				return type;

		return nullptr;
	}

	const std::vector<const Type*>& Debug::GetTypes()
	{
		return sTypes;
	}
}