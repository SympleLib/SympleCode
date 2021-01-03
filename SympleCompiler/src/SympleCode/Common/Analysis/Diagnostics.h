#pragma once

#include <vector>

#include "SympleCode/Common/Token.h"

#define DIAGNOSTIC_LEVEL_ERROR    2
#define DIAGNOSTIC_LEVEL_WARNING  1

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
		static std::vector<const Message*> sMessages;
		static std::vector<const Message*> sWarnings;
		static std::vector<const Message*> sErrors;
	public:
		static void Clear();

		static void ReportError(const Token* token, const char* fmt, ...);
		static void ReportWarning(const Token* token, const char* fmt, ...);

		static const std::vector<const Message*>& GetMessages();
		static const std::vector<const Message*>& GetWarnings();
		static const std::vector<const Message*>& GetErrors();
	};
}