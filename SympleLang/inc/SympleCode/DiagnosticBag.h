#pragma once

#include <vector>

#include "SympleCode/Diagnostic.h"

#include "SympleCode/Syntax/Token.h"
#include "SympleCode/Syntax/LiteralExpressionSyntax.h"

#include "SympleCode/Binding/Type.h"

namespace Symple
{
	class DiagnosticBag
	{
	private:
		std::vector<shared_ptr<Diagnostic>> mDiagnostics;
	public:
		void ReportMessage(shared_ptr<Syntax::Token>, std::string_view msg);
		void ReportWarning(shared_ptr<Syntax::Token>, std::string_view msg);
		void ReportError(shared_ptr<Syntax::Token>, std::string_view msg);

		std::vector<shared_ptr<Diagnostic>>& GetDiagnostics();

		void ReportUnexpectedEndOfFile(shared_ptr<Syntax::Token>);
		void ReportUnexpectedToken(shared_ptr<Syntax::Token>, Syntax::Token::Kind expectedKind);
		void ReportUnknownToken(shared_ptr<Syntax::Token>);

		void ReportInvalidOperation(shared_ptr<Syntax::Token>, shared_ptr<Binding::Type> left, shared_ptr<Binding::Type> right);
		void ReportInvalidOperation(shared_ptr<Syntax::Token>, shared_ptr<Binding::Type>);

		void ReportInvalidLiteral(shared_ptr<Syntax::LiteralExpressionSyntax>);
	};
}