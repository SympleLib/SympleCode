#pragma once

#include <vector>

#include "SympleCode/Diagnostic.h"

#include "SympleCode/Syntax/LiteralExpressionNode.h"

namespace Symple
{
	class DiagnosticBag
	{
	private:
		std::vector<std::shared_ptr<Diagnostic>> mDiagnostics;
	public:
		void ReportMessage(std::shared_ptr<Syntax::Token>, std::string_view msg);
		void ReportWarning(std::shared_ptr<Syntax::Token>, std::string_view msg);
		void ReportError(std::shared_ptr<Syntax::Token>, std::string_view msg);

		void ReportUnexpectedToken(std::shared_ptr<Syntax::Token>);
		void ReportInvalidLiteral(std::shared_ptr<Syntax::LiteralExpressionNode>);
	};
}