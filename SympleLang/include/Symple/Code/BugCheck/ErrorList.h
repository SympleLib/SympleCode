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
	};

	struct SYC_API ErrorMessage
	{
		const std::string Msg;
		const ErrorLevel Severity;
		const WeakRef<const Token> Tok;
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


		const std::vector<GlobalRef<const ErrorMessage>> &GetMessages();

		const std::vector<GlobalRef<const ErrorMessage>> &GetFatalMessages();
		const std::vector<GlobalRef<const ErrorMessage>> &GetErrorMessages();
		const std::vector<GlobalRef<const ErrorMessage>> &GetWarningMessages();
		const std::vector<GlobalRef<const ErrorMessage>> &GetMessageMessages();
	};
}