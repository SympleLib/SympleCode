#include "SympleCode/Analysis/Diagnostics.h"

#include <cstdarg>
#include <iostream>

#define GENERATE_MESSAGE \
	char message[128]; \
	va_list args; \
	va_start(args, fmt); \
	vsprintf_s(message, fmt, args); \
	va_end(args)

namespace Symple
{
	std::vector<const Message*> Diagnostics::sMessages;
	std::vector<const Message*> Diagnostics::sWarnings;
	std::vector<const Message*> Diagnostics::sErrors;

	void Diagnostics::Clear()
	{
		sMessages.clear();
		sWarnings.clear();
		sErrors.clear();
	}

	void Diagnostics::ReportError(const Token* token, const char* fmt, ...)
	{
		GENERATE_MESSAGE;

		sMessages.push_back(new Message{ message, DIAGNOSTIC_LEVEL_ERROR, token });
		sErrors.push_back(sMessages.back());
	}

	void Diagnostics::ReportWarning(const Token* token, const char* fmt, ...)
	{
		GENERATE_MESSAGE;

		sMessages.push_back(new Message{ message, DIAGNOSTIC_LEVEL_WARNING, token });
		sWarnings.push_back(sMessages.back());
	}

	void Diagnostics::ReportError(bool condition, const Token* token, const char* fmt, ...)
	{
		if (!condition)
			return;

		GENERATE_MESSAGE;

		sMessages.push_back(new Message{ message, DIAGNOSTIC_LEVEL_ERROR, token });
		sErrors.push_back(sMessages.back());
	}

	void Diagnostics::ReportWarning(bool condition, const Token* token, const char* fmt, ...)
	{
		if (!condition)
			return;

		GENERATE_MESSAGE;

		sMessages.push_back(new Message{ message, DIAGNOSTIC_LEVEL_WARNING, token });
		sWarnings.push_back(sMessages.back());
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
}