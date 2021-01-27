#include "SympleCode/DiagnosticBag.h"

namespace Symple
{
	void DiagnosticBag::ReportMessage(std::shared_ptr<Syntax::Token> tok, std::string_view msg)
	{ mDiagnostics.push_back(make_shared<Diagnostic>(Diagnostic::Message, tok, msg)); }

	void DiagnosticBag::ReportWarning(std::shared_ptr<Syntax::Token> tok, std::string_view msg)
	{ mDiagnostics.push_back(make_shared<Diagnostic>(Diagnostic::Warning, tok, msg)); }

	void DiagnosticBag::ReportError(std::shared_ptr<Syntax::Token> tok, std::string_view msg)
	{ mDiagnostics.push_back(make_shared<Diagnostic>(Diagnostic::Error, tok, msg)); }
}