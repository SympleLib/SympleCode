#include "SympleCode/Common/Analysis/Diagnostics.h"

#include <cstdarg>

namespace Symple
{
	Diagnostics* Diagnostics::sDiagnostics = new Diagnostics;

	void Diagnostics::ReportError(const Token* token, const char* fmt, ...)
	{
		char message[128];
		va_list args;
		va_start(args, fmt);
		vsprintf_s(message, fmt, args);
		va_end(args);

		mMessages.push_back(new Message{ message, DIAGNOSTIC_LEVEL_ERROR, token });
		mErrors.push_back(mMessages.back());
	}

	void Diagnostics::ReportWarning(const Token* token, const char* fmt, ...)
	{
		char message[128];
		va_list args;
		va_start(args, fmt);
		vsprintf_s(message, fmt, args);
		va_end(args);

		mMessages.push_back(new Message{ message, DIAGNOSTIC_LEVEL_WARNING, token });
		mWarnings.push_back(mMessages.back());
	}

	void Diagnostics::FunctionDeclaration(const FunctionDeclarationNode* function)
	{
		mFunctions.insert({ function->GetName()->GetLex(), function });
	}

	void Diagnostics::VariableDeclaration(const VariableDeclarationNode* variable)
	{
		mVariables.insert({ variable->GetName()->GetLex(), variable });
	}

	void Diagnostics::BeginScope()
	{
		pVariables = mVariables;
	}

	void Diagnostics::EndScope()
	{
		mVariables = pVariables;
	}

	const std::vector<const Message*>& Diagnostics::GetMessages() const
	{
		return mMessages;
	}

	const std::vector<const Message*>& Diagnostics::GetErrors() const
	{
		return mErrors;
	}

	const std::vector<const Message*>& Diagnostics::GetWarnings() const
	{
		return mWarnings;
	}

	const FunctionDeclarationNode* Diagnostics::GetFunction(const std::string_view& name, const FunctionCallArgumentsNode* arguments) const
	{
		for (const auto& function : mFunctions)
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

	const FunctionDeclarationNode* Diagnostics::GetFunction(const FunctionCallExpressionNode* call) const
	{
		return GetFunction(call->GetName()->GetLex(), call->GetArguments());
	}

	const std::map<std::string_view, const FunctionDeclarationNode*>& Diagnostics::GetFunctions() const
	{
		return mFunctions;
	}

	const VariableDeclarationNode* Diagnostics::GetVariable(const std::string_view& name) const
	{
		if (mVariables.find(name) == mVariables.end())
			return mVariables.at(name);
		return nullptr;
	}

	const std::map<std::string_view, const VariableDeclarationNode*>& Diagnostics::GetVariables() const
	{
		return mVariables;
	}
}