#include "Symple/Code/BugCheck/ErrorList.h"

namespace Symple::Code
{
	void ErrorList::Report(const GlobalRef<const ErrorMessage> &err)
	{
		m_Msgs.push_back(err);

		switch (err->Severity)
		{
		case ErrorLevel::Fatal:
			m_FatalMsgs.push_back(err);
			break;
		case ErrorLevel::Error:
			m_ErrorMsgs.push_back(err);
			break;
		case ErrorLevel::Warning:
			m_WarningMsgs.push_back(err);
			break;
		case ErrorLevel::Message:
			m_MessageMsgs.push_back(err);
			break;
		}
	}
	
	
	void ErrorList::ReportWrongToken(const GlobalRef<const Token> &tok, TokenKind expected)
	{
		Report("");
	}


	const std::vector<GlobalRef<const ErrorMessage>> &ErrorList::GetMessages()
	{ return m_Msgs; }

	const std::vector<GlobalRef<const ErrorMessage>> &ErrorList::GetFatalMessages()
	{ return m_FatalMsgs; }

	const std::vector<GlobalRef<const ErrorMessage>> &ErrorList::GetErrorMessages()
	{ return m_ErrorMsgs; }

	const std::vector<GlobalRef<const ErrorMessage>> &ErrorList::GetWarningMessages()
	{ return m_WarningMsgs; }

	const std::vector<GlobalRef<const ErrorMessage>> &ErrorList::GetMessageMessages()
	{ return m_MessageMsgs; }
}