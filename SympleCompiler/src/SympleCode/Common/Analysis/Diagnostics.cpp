#include "SympleCode/Common/Analysis/Diagnostics.h"

#include <cstdarg>

namespace Symple
{
	std::vector<const Message*> Diagnostics::sMessages;
	std::vector<const Message*> Diagnostics::sWarnings;
	std::vector<const Message*> Diagnostics::sErrors;

	std::map<std::string_view, const FunctionDeclarationNode*> Diagnostics::sFunctions;
	std::map<std::string_view, const VariableDeclarationNode*> Diagnostics::sVariables;
	std::map<std::string_view, const VariableDeclarationNode*> Diagnostics::pVariables;

	void Diagnostics::ReportError(const Token* token, const char* fmt, ...)
	{
		char message[128];
		va_list args;
		va_start(args, fmt);
		vsprintf_s(message, fmt, args);
		va_end(args);

		sMessages.push_back(new Message{ message, DIAGNOSTIC_LEVEL_ERROR, token });
		sErrors.push_back(sMessages.back());
	}

	void Diagnostics::ReportWarning(const Token* token, const char* fmt, ...)
	{
		char message[128];
		va_list args;
		va_start(args, fmt);
		vsprintf_s(message, fmt, args);
		va_end(args);

		sMessages.push_back(new Message{ message, DIAGNOSTIC_LEVEL_WARNING, token });
		sWarnings.push_back(sMessages.back());
	}

	void Diagnostics::FunctionDeclaration(const FunctionDeclarationNode* function)
	{
		sFunctions.insert({ function->GetName()->GetLex(), function });
	}

	void Diagnostics::VariableDeclaration(const VariableDeclarationNode* variable)
	{
		sVariables.insert({ variable->GetName()->GetLex(), variable });
	}

	void Diagnostics::BeginScope()
	{
		pVariables = sVariables;
	}

	void Diagnostics::EndScope()
	{
		sVariables = pVariables;
	}

	const std::vector<const Message*>& Diagnostics::GetMessages()
	{
		return sMessages;
	}

	const std::vector<const Message*>& Diagnostics::GetErrors()
	{
		return sErrors;
	}

	const std::vector<const Message*>& Diagnostics::GetWarnings()
	{
		return sWarnings;
	}

	const FunctionDeclarationNode* Diagnostics::GetFunction(const std::string_view& name, const FunctionCallArgumentsNode* arguments)
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

	const std::map<std::string_view, const FunctionDeclarationNode*>& Diagnostics::GetFunctions()
	{
		return sFunctions;
	}

	const VariableDeclarationNode* Diagnostics::GetVariable(const std::string_view& name)
	{
		if (sVariables.find(name) != sVariables.end())
			return sVariables.at(name);
		return nullptr;
	}

	const std::map<std::string_view, const VariableDeclarationNode*>& Diagnostics::GetVariables()
	{
		return sVariables;
	}
}