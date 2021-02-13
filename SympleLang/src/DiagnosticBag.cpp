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


#if __SY_ALLOW_UNIMPLIMENTED
	void DiagnosticBag::ReportUnimplimentedMessage(shared_ptr<Syntax::Token> tok)
	{ ReportWarning(tok, "message not implemented"); }

	void DiagnosticBag::ReportUnimplimentedWarning(shared_ptr<Syntax::Token> tok)
	{ ReportWarning(tok, "warning not implemented"); }

	void DiagnosticBag::ReportUnimplimentedError(shared_ptr<Syntax::Token> tok)
	{ ReportWarning(tok, "error not implemented"); }
#endif


	void DiagnosticBag::ReportBindError(shared_ptr<Syntax::Node> syntax)
	{ ReportError(syntax->GetToken(), "bind error"); }
	
	void DiagnosticBag::ReportExpressionMustHaveValue(shared_ptr<Syntax::ExpressionSyntax> syntax)
	{ ReportError(syntax->GetToken(), "expression must have a value"); }


	void DiagnosticBag::ReportNoDefaultArgument(shared_ptr<Syntax::CallExpressionSyntax> syntax, unsigned param)
	{ ReportError(syntax->GetCloseParenthesis(), fmt::format("no default argument for parameter {}", param + 1)); }

	void DiagnosticBag::ReportTooFewArguments(shared_ptr<Syntax::CallExpressionSyntax> syntax)
	{ ReportError(syntax->GetCloseParenthesis(), "too few Arguments"); }

	void DiagnosticBag::ReportTooManyArguments(shared_ptr<Syntax::CallExpressionSyntax> syntax, unsigned param)
	{ ReportError(syntax->GetArguments()[param]->GetToken(), "too many arguments"); }

	void DiagnosticBag::ReportNoSuchFunction(shared_ptr<Syntax::CallExpressionSyntax> syntax)
	{ ReportError(syntax->GetToken(), "function Doesn't Exist"); }


	void DiagnosticBag::ReportUnexpectedEndOfFile(shared_ptr<Syntax::Token> tok)
	{ ReportError(tok, "unexpected end of file"); }

	void DiagnosticBag::ReportUnexpectedDllImportBody(shared_ptr<Syntax::Token> tok)
	{ ReportError(tok, "unexpected dllimport body"); }

	void DiagnosticBag::ReportUndeclaredIdentifier(shared_ptr<Syntax::Token> tok)
	{ ReportError(tok, "undeclared identifier"); }

	void DiagnosticBag::ReportUnexpectedToken(shared_ptr<Token> tok, Token::Kind expected)
	{
		std::stringstream tokStr;
		tok->PrintShort(tokStr);
		ReportError(tok, fmt::format("unexpected token '{}', expected {}", tokStr.str(), Token::KindMap[expected]));
	}

	void DiagnosticBag::ReportUnknownToken(shared_ptr<Syntax::Token> tok)
	{ ReportError(tok, fmt::format("unknown token '{}'", tok->GetText())); }


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
		ReportError(tok, fmt::format("invalid operation [{}] of types {}, and {}", tokstr, lstr, rstr));
	}

	void DiagnosticBag::ReportInvalidOperation(shared_ptr<Syntax::Token> tok, shared_ptr<Symbol::TypeSymbol> ty)
	{
		std::stringstream ss;
		tok->PrintShort(ss);
		std::string tokstr = ss.str();

		ss.str({}); // Clear stream
		ty->PrintShort(ss);
		std::string tystr = ss.str();
		ReportError(tok, fmt::format("invalid operation [{}] of type {}", tokstr, tystr));
	}


	void DiagnosticBag::ReportExpectedUnqualifiedID(shared_ptr<Syntax::Token> tok)
	{ ReportError(tok, "expected unqualidied-id"); }

	void DiagnosticBag::ReportExpectedLValue(shared_ptr<Syntax::Token> tok)
	{ ReportError(tok, "expected lvalue"); }

	void DiagnosticBag::ReportInvalidLiteral(shared_ptr<LiteralExpressionSyntax> literal)
	{
		std::stringstream tokStr;
		literal->GetLiteral()->PrintShort(tokStr);
		ReportError(literal->GetLiteral(), fmt::format("invalid literal '{}'", tokStr.str()));
	}
}