#include "SympleCode/Common/Analysis/Debug.h"

#include <cstdarg>
#include <iostream>

namespace Symple
{
	std::map<std::string_view, const FunctionDeclarationNode*> Debug::sFunctions;
	std::map<std::string_view, const VariableDeclarationNode*> Debug::sVariables;
	std::map<std::string_view, const VariableDeclarationNode*> Debug::pVariables;

	void Debug::Clear()
	{
		sFunctions.clear();
		sVariables.clear();
		pVariables.clear();
	}

	void Debug::FunctionDeclaration(const FunctionDeclarationNode* function)
	{
		sFunctions.insert({ function->GetName()->GetLex(), function });
	}

	void Debug::VariableDeclaration(const VariableDeclarationNode* variable)
	{
		sVariables.insert({ variable->GetName()->GetLex(), variable });
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
		for (const auto& function : sFunctions)
		{
			if (function.first == name)
			{
				if (function.second->GetModifiers()->GetFormatType())
					switch (function.second->GetModifiers()->GetFormatType()->GetModifier()->GetKind())
					{
					case Token::Kind::SympleCall:
						goto SympleCall;
					case Token::Kind::StdCall:
					case Token::Kind::CCall:
						return function.second;
					}

			SympleCall:
				bool same = true;

				for (unsigned int i = 0; i < arguments->GetArguments().size(); i++)
				{
					if (i >= function.second->GetArguments()->GetArguments().size())
					{
						same = false;

						break;
					}

					same &= arguments->GetArguments()[i]->GetType()->GetType() == function.second->GetArguments()->GetArguments()[i]->GetType()->GetType();
				}

				if (same)
					return function.second;
			}
		}

		return nullptr;
	}

	const std::map<std::string_view, const FunctionDeclarationNode*>& Debug::GetFunctions()
	{
		return sFunctions;
	}

	const VariableDeclarationNode* Debug::GetVariable(const std::string_view& name)
	{
		if (sVariables.find(name) == sVariables.end())
			return nullptr;
		return sVariables.at(name);
	}

	const std::map<std::string_view, const VariableDeclarationNode*>& Debug::GetVariables()
	{
		return sVariables;
	}
}