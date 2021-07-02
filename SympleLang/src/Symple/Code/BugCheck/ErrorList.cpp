#include "Symple/Code/BugCheck/ErrorList.h"

#include "Symple/Code/Util/Format.h"

namespace Symple::Code
{
	SYC_API std::ostream &operator <<(std::ostream &os, ErrorLevel lvl)
	{ return os << ErrorLevelNames[(uint32)lvl]; }

	void ErrorMessage::Print(std::ostream &os) const
	{
		os << '[' << Severity << ']';
		if (!Tok.expired())
		{
			auto tok = Tok.lock();
			os << '(' << tok->DisplayLine << ':' << tok->Column << ')';
		}
		os << ": " << Msg;
	}


	GlobalRef<const ErrorMessage> ErrorList::Report(const GlobalRef<const ErrorMessage> &err)
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

		return err;
	}
	

	GlobalRef<const ErrorMessage> ErrorList::ReportExpectedExpression(const GlobalRef<const Token> &tok)
	{ return Report("Expected expression", ErrorLevel::Error, tok); }

	GlobalRef<const ErrorMessage> ErrorList::ReportWrongToken(const GlobalRef<const Token> &tok, TokenKind expected)
	{ return Report(Format("Unexpected {} '{}', expected {}", tok->Kind, tok->Text, expected), ErrorLevel::Error, tok); }

	GlobalRef<const ErrorMessage> ErrorList::ReportEndOfFile(const GlobalRef<const Token> &tok)
	{ return Report("Unexpected EndOfFile", ErrorLevel::Error, tok); }

	GlobalRef<const ErrorMessage> ErrorList::ReportUnresolvedSymbol(const GlobalRef<const Token> &tok)
	{ return Report(Format("Unresolved symbol: `{}`", tok->Text), ErrorLevel::Error, tok); }


	void ErrorList::Clear()
	{
		m_Msgs.clear();
		m_FatalMsgs.clear();
		m_ErrorMsgs.clear();
		m_WarningMsgs.clear();
		m_MessageMsgs.clear();
	}

	void ErrorList::Dump(std::ostream &os)
	{
		for (auto msg : m_Msgs)
		{ msg->Print(os); os << '\n'; }
	}

	bool ErrorList::IsEmpty()
	{ return m_Msgs.empty(); }


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