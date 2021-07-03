#include "Symple/Code/Parse/Facts.h"

namespace Symple::Code
{
	SYC_API std::vector<const TypeBase &> TokenFacts::TypeBases;

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

	const TypeBase &TokenFacts::GetTypeBase(TokenKind kind)
	{
		for (const auto &bases : TypeBases)
			if (bases.Name == )

		throw nullptr;
	}

	bool TokenFacts::IsTypeBase(TokenKind kind)
	{
		switch (kind)
		{
		case TokenKind::VoidKeyword:
		case TokenKind::ByteKeyword:
		case TokenKind::ShortKeyword:
		case TokenKind::IntKeyword:
		case TokenKind::LongKeyword:
		case TokenKind::FloatKeyword:
		case TokenKind::DoubleKeyword:
		case TokenKind::CharKeyword:
		case TokenKind::WCharKeyword:
		case TokenKind::BoolKeyword:
			return true;

		default:
			return false;
		}
	}

	bool TokenFacts::IsTypePointer(TokenKind kind)
	{ return kind == TokenKind::Star; }

	bool TokenFacts::IsTypeArray(TokenKind kind)
	{ return kind == TokenKind::Carot; }


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