#include "SympleCode/DiagnosticBag.h"

#include <sstream>

#include <spdlog/fmt/fmt.h>

namespace Symple
{
	using namespace Syntax;

	void DiagnosticBag::ReportMessage(shared_ptr<Token> tok, std::string_view msg)
	{
		mDiagnostics.push_back(make_shared<Diagnostic>(Diagnostic::Message, tok, msg));
	}

	void DiagnosticBag::ReportWarning(shared_ptr<Token> tok, std::string_view msg)
	{
		mDiagnostics.push_back(make_shared<Diagnostic>(Diagnostic::Warning, tok, msg));
	}

	void DiagnosticBag::ReportError(shared_ptr<Token> tok, std::string_view msg)
	{
		mDiagnostics.push_back(make_shared<Diagnostic>(Diagnostic::Error, tok, msg));
	}


	std::vector<shared_ptr<Diagnostic>>& DiagnosticBag::GetDiagnostics()
	{ return mDiagnostics; }


	void DiagnosticBag::ReportUnexpectedEndOfFile(shared_ptr<Syntax::Token> tok)
	{
		ReportError(tok, fmt::format("Unexpected end of file"));
	}

	void DiagnosticBag::ReportUnexpectedToken(shared_ptr<Token> tok, Token::Kind expected)
	{
		std::stringstream tokStr;
		tok->PrintShort(tokStr);
		ReportError(tok, fmt::format("Unexpected token '{}', expected {}", tokStr.str(), Token::KindMap[expected]));
	}

	void DiagnosticBag::ReportUnknownToken(shared_ptr<Syntax::Token> tok)
	{ ReportError(tok, fmt::format("Unknown token '{}'", tok->GetText())); }


	void DiagnosticBag::ReportInvalidLiteral(shared_ptr<LiteralExpressionNode> literal)
	{
		std::stringstream tokStr;
		literal->GetLiteral()->PrintShort(tokStr);
		ReportError(literal->GetLiteral(), fmt::format("Invalid literal '{}'", tokStr.str()));
	}
}