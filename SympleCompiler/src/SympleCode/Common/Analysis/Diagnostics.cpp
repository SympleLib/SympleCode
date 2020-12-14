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
	}

	void Diagnostics::ReportWarning(const Token* token, const char* fmt, ...)
	{
		char message[128];
		va_list args;
		va_start(args, fmt);
		vsprintf_s(message, fmt, args);
		va_end(args);

		mMessages.push_back(new Message{ message, DIAGNOSTIC_LEVEL_WARNING, token });
	}

	const std::vector<const Message*>& Diagnostics::GetMessages() const
	{
		return mMessages;
	}
}