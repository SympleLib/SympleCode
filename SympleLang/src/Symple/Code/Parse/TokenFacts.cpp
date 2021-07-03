#include "Symple/Code/Parse/Facts.h"

namespace Symple::Code
{
	bool TokenFacts::IsBuiltinFunction(TokenKind kind)
	{
		switch (kind)
		{
		case TokenKind::SizeofKeyword:
		case TokenKind::LengthofKeyword:
			return true;

		default:
			return false;
		}
	}

	bool TokenFacts::IsTypePointer(TokenKind kind)
	{ return kind == TokenKind::Star; }


	bool TokenFacts::IsFuncMod(TokenKind kind)
	{
		switch (kind)
		{
		case TokenKind::CCallKeyword:
		case TokenKind::StdCallKeyword:
		case TokenKind::SyCallKeyword:
		case TokenKind::SycCallKeyword:
			return true;

		default:
			return false;
		}
	}
}