#pragma once

#include <vector>
#include <string>

#include "SympleCode/Common/Token.h"

#define DIAGNOSTIC_LEVEL_ERROR    2
#define DIAGNOSTIC_LEVEL_WARNING  1
#define DIAGNOSTIC_LEVEL_MESSAGE  0

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
		std::vector<const Message*> mMessages;
	public:
		void ReportError(const Token* token, const char* fmt, ...);

		void ReportWarning(const Token* token, const char* fmt, ...);

		void ReportMessage(const Token* token, const char* fmt, ...);

		const std::vector<const Message*>& GetMessages() const;
	};
}