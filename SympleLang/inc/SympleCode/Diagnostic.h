#pragma once

#include "SympleCode/Syntax/Token.h"

namespace Symple
{
	class Diagnostic
	{
	public: enum Level : unsigned;
	private:
		Level mLevel;
		shared_ptr<Syntax::Token> mToken;
		std::string mMessage;
	public:
		Diagnostic(Level lvl, shared_ptr<Syntax::Token> tok, std::string_view msg)
			: mLevel(lvl), mToken(tok), mMessage(msg) {}

		Level GetLevel()
		{ return mLevel; }

		auto GetToken()
		{ return mToken; }
	public:
		enum Level : unsigned
		{
			Message,
			Warning,
			Error,
		};
	};
}