#include "SympleCode/DiagnosticBag.h"

#include <sstream>

#include <spdlog/fmt/fmt.h>

namespace Symple
{
	using namespace Syntax;

	void DiagnosticBag::ReportMessage(shared_ptr<Token> tok, std::string_view msg)
	{ mDiagnostics.push_back(make_shared<Diagnostic>(Diagnostic::Message, tok, msg)); }

	void DiagnosticBag::ReportWarning(shared_ptr<Token> tok, std::string_view msg)
	{ mDiagnostics.push_back(make_shared<Diagnostic>(Diagnostic::Warning, tok, msg)); }

	void DiagnosticBag::ReportError(shared_ptr<Token> tok, std::string_view msg)
	{ mDiagnostics.push_back(make_shared<Diagnostic>(Diagnostic::Error, tok, msg)); }


	std::vector<shared_ptr<Diagnostic>>& DiagnosticBag::GetDiagnostics()
	{ return mDiagnostics; }


	void DiagnosticBag::ReportUnexpectedEndOfFile(shared_ptr<Syntax::Token> tok)
	{ ReportError(tok, fmt::format("Unexpected end of file")); }

	void DiagnosticBag::ReportUnexpectedToken(shared_ptr<Token> tok, Token::Kind expected)
	{
		std::stringstream tokStr;
		tok->PrintShort(tokStr);
		ReportError(tok, fmt::format("Unexpected token '{}', expected {}", tokStr.str(), Token::KindMap[expected]));
	}

	void DiagnosticBag::ReportUnknownToken(shared_ptr<Syntax::Token> tok)
	{ ReportError(tok, fmt::format("Unknown token '{}'", tok->GetText())); }


	void DiagnosticBag::ReportInvalidOperation(shared_ptr<Syntax::Token> tok, shared_ptr<Binding::Type> l, shared_ptr<Binding::Type> r)
	{
		std::stringstream ss;
		tok->PrintShort(ss);
		std::string tokstr = ss.str();
		
		ss.str({}); // Clear stream
		l->PrintShort(ss);
		std::string lstr = ss.str();

		ss.str({}); // Clear stream
		r->PrintShort(ss);
		std::string rstr = ss.str();
		ReportError(tok, fmt::format("Invalid operation [{}] of types {}, and {}", tokstr, lstr, rstr));
	}

	void DiagnosticBag::ReportInvalidOperation(shared_ptr<Syntax::Token> tok, shared_ptr<Binding::Type> ty)
	{
		std::stringstream ss;
		tok->PrintShort(ss);
		std::string tokstr = ss.str();

		ss.str({}); // Clear stream
		ty->PrintShort(ss);
		std::string tystr = ss.str();
		ReportError(tok, fmt::format("Invalid operation [{}] of type {}", tokstr, tystr));
	}


	void DiagnosticBag::ReportExpectedUnqualifiedID(shared_ptr<Syntax::Token> tok)
	{ ReportError(tok, "Expected unqualidied-id"); }

	void DiagnosticBag::ReportInvalidLiteral(shared_ptr<LiteralExpressionSyntax> literal)
	{
		std::stringstream tokStr;
		literal->GetLiteral()->PrintShort(tokStr);
		ReportError(literal->GetLiteral(), fmt::format("Invalid literal '{}'", tokStr.str()));
	}
}