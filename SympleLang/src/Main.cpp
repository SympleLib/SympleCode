#include <iostream>
#include <cstdlib>

#include <spdlog/spdlog.h>

#include "SympleCode/Syntax/Lexer.h"
#include "SympleCode/Syntax/Parser.h"
#include "SympleCode/Binding/Binder.h"

using Symple::DiagnosticBag;
using Symple::Diagnostic;

using Symple::Syntax::Lexer;
using Symple::Syntax::Token;
using Symple::Syntax::Parser;
using Symple::Binding::Binder;
using Symple::Binding::BoundExpression;

using Symple::Syntax::ExpressionSyntax;

using std::shared_ptr;
using std::make_shared;

using std::unique_ptr;
using std::make_unique;

using spdlog::level::level_enum;

std::string_view sStep = "Null Step";

bool PrintDiagnosticBag(shared_ptr<DiagnosticBag> diagnostics)
{
	unsigned errCount = 0, warningCount = 0, messageCount = 0;

	for (shared_ptr<Diagnostic> diagnostic : diagnostics->GetDiagnostics())
		switch (diagnostic->GetLevel())
		{
		case Diagnostic::Message:
			spdlog::info("{}", diagnostic->GetMessage());
			messageCount++;
			break;
		case Diagnostic::Warning:
			spdlog::warn("{}", diagnostic->GetMessage());
			warningCount++;
			break;
		case Diagnostic::Error:
			spdlog::error("{}", diagnostic->GetMessage());
			errCount++;
			break;
		}

	if (errCount)
	{
		spdlog::info("{} failed with {} errors, {} warnings, {} messages", sStep, errCount, warningCount, messageCount);

		return true;
	}
	else
		return false;
}

shared_ptr<ExpressionSyntax> node;

void Parse()
{
	spdlog::set_pattern("[Symple]%^<%l>%$: %v");
	spdlog::set_level(level_enum::trace);

	shared_ptr<Lexer> lexer = make_shared<Lexer>((char*)"sy/Main.sy");
	lexer->SetRef(lexer);
	std::vector<shared_ptr<Token>> tokens;
	do
		tokens.push_back(lexer->Lex());
	while (!tokens.back()->Is(Token::EndOfFile));

	sStep = "Lexing";
	if (PrintDiagnosticBag(lexer->GetDiagnosticBag()))
		return;

	shared_ptr<Parser> parser = make_shared<Parser>(lexer, tokens);
	node = parser->ParseExpression();
	sStep = "Parsing";
	if (PrintDiagnosticBag(parser->GetDiagnosticBag()))
		return;

	node->Print();
	putchar('\n');
}

void Bind()
{
	if (!node)
		return;

	shared_ptr<Binder> binder = make_shared<Binder>();

	{
		shared_ptr<BoundExpression> bound = binder->BindExpression(node);

		bound->Print();
		putchar('\n');
	}


}

int main()
{
	Parse();
	Bind();

	return !getc(stdin);
}