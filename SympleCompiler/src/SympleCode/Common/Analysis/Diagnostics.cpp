#include "SympleCode/Common/Analysis/Diagnostics.h"

#include <cstdarg>

namespace Symple
{
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

	const FunctionDeclarationNode* Diagnostics::GetFunction(const std::string_view& name) const
	{
		return mFunctions.at(name);
	}

	const std::map<std::string_view, const FunctionDeclarationNode*>& Diagnostics::GetFunctions() const
	{
		return mFunctions;
	}
}