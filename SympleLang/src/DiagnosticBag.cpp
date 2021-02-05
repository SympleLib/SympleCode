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


	void DiagnosticBag::ReportUnimplimentedMessage(shared_ptr<Syntax::Token> tok)
	{ ReportWarning(tok, "Message not implemented"); }

	void DiagnosticBag::ReportUnimplimentedWarning(shared_ptr<Syntax::Token> tok)
	{ ReportWarning(tok, "Warning not implemented"); }

	void DiagnosticBag::ReportUnimplimentedError(shared_ptr<Syntax::Token> tok)
	{ ReportWarning(tok, "Error not implemented"); }


	void DiagnosticBag::ReportBindError(shared_ptr<Syntax::Node> syntax)
	{ ReportWarning(syntax->GetToken(), "Bind error"); }
	
	void DiagnosticBag::ReportExpressionMustHaveValue(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{ ReportWarning(syntax->GetToken(), "Expression must have a value"); }


	void DiagnosticBag::ReportNoDefaultArgument(shared_ptr<Syntax::CallExpressionSyntax> syntax, unsigned param)
	{ ReportWarning(syntax->GetCloseParenthesis(), fmt::format("No default argument for parameter {}", param)); }

	void DiagnosticBag::ReportTooFewArguments(shared_ptr<Syntax::CallExpressionSyntax> syntax)
	{ ReportWarning(syntax->GetCloseParenthesis(), "Too few Arguments"); }

	void DiagnosticBag::ReportTooManyArguments(shared_ptr<Syntax::CallExpressionSyntax> syntax, unsigned param)
	{ ReportWarning(syntax->GetArguments()[param]->GetToken(), "Too many arguments"); }


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


	void DiagnosticBag::ReportInvalidOperation(shared_ptr<Syntax::Token> tok, shared_ptr<Symbol::TypeSymbol> l, shared_ptr<Symbol::TypeSymbol> r)
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

	void DiagnosticBag::ReportInvalidOperation(shared_ptr<Syntax::Token> tok, shared_ptr<Symbol::TypeSymbol> ty)
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