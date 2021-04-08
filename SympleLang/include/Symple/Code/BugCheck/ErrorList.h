#pragma once

#include <string>

#include "Symple/Code/Common.h"
#include "Symple/Code/Parse/Token.h"
#include "Symple/Code/Util/Memory.h"

namespace Symple::Code
{
	enum class SYC_API ErrorLevel
	{
		Message,
		Warning,
		Error,

		Fatal, // Should never be emitted

		Last = Fatal,
		Count,
	};

	constexpr const char *const ErrorLevelNames[(uint32)ErrorLevel::Count] =
	{
		"Message",
		"Warning",
		"Error",
		"Fatal",
	};

	SYC_API std::ostream &operator <<(std::ostream &, ErrorLevel);

	struct SYC_API ErrorMessage
	{
		const std::string Msg;
		const ErrorLevel Severity;
		const WeakRef<const Token> Tok;

		void Print(std::ostream &) const;
	};

	class SYC_API ErrorList
	{
	private:
		std::vector<GlobalRef<const ErrorMessage>> m_Msgs;

		std::vector<GlobalRef<const ErrorMessage>> m_FatalMsgs;
		std::vector<GlobalRef<const ErrorMessage>> m_ErrorMsgs;
		std::vector<GlobalRef<const ErrorMessage>> m_WarningMsgs;
		std::vector<GlobalRef<const ErrorMessage>> m_MessageMsgs;
	public:
		void Report(const GlobalRef<const ErrorMessage> &);
		void Report(const std::string &message, ErrorLevel severity, const WeakRef<const Token> &token)
		{ Report(MakeRef<ErrorMessage>(message, severity, token)); }

		void ReportWrongToken(const GlobalRef<const Token> &, TokenKind expected);


		void Dump(std::ostream &);
		bool IsEmpty();

		const std::vector<GlobalRef<const ErrorMessage>> &GetMessages();

		const std::vector<GlobalRef<const ErrorMessage>> &GetFatalMessages();
		const std::vector<GlobalRef<const ErrorMessage>> &GetErrorMessages();
		const std::vector<GlobalRef<const ErrorMessage>> &GetWarningMessages();
		const std::vector<GlobalRef<const ErrorMessage>> &GetMessageMessages();


		SY_PROPERTY_GET(GetMessages) const std::vector<GlobalRef<const ErrorMessage>> &Messages;

		SY_PROPERTY_GET(GetFatalMessages) const std::vector<GlobalRef<const ErrorMessage>> &FatalMessages;
		SY_PROPERTY_GET(GetErrorMessages) const std::vector<GlobalRef<const ErrorMessage>> &ErrorMessages;
		SY_PROPERTY_GET(GetWarningMessages) const std::vector<GlobalRef<const ErrorMessage>> &WarningMessages;
		SY_PROPERTY_GET(GetMessageMessages) const std::vector<GlobalRef<const ErrorMessage>> &MessageMessages;
	};
}