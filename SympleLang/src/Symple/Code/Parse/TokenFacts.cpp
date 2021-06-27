#include "Symple/Code/Parse/Facts.h"

namespace Symple::Code
{
	TypeKind TokenFacts::ToType(TokenKind kind)
	{
		switch (kind)
		{
		case TokenKind::VoidKeyword:
			return TypeKind::Void;
		case TokenKind::ByteKeyword:
			return TypeKind::Byte;
		case TokenKind::ShortKeyword:
			return TypeKind::Short;
		case TokenKind::IntKeyword:
			return TypeKind::Int;
		//case TokenKind::LongKeyword:
			//return TypeKind::Long;
		case TokenKind::FloatKeyword:
			return TypeKind::Float;
		//case TokenKind::DoubleKeyword:
			//return TypeKind::Double;
		//case TokenKind::TripleKeyword:
			//return TypeKind::Triple;
		case TokenKind::CharKeyword:
			return TypeKind::Char;
		case TokenKind::WCharKeyword:
			return TypeKind::WChar;
		case TokenKind::BoolKeyword:
			return TypeKind::Bool;

		default:
			throw std::exception();
		}
	}

	bool TokenFacts::IsTypeBase(TokenKind kind)
	{
		switch (kind)
		{
		case TokenKind::VoidKeyword:
		case TokenKind::ByteKeyword:
		case TokenKind::ShortKeyword:
		case TokenKind::IntKeyword:
		//case TokenKind::LongKeyword:
		case TokenKind::FloatKeyword:
		//case TokenKind::DoubleKeyword:
		//case TokenKind::TripleKeyword:
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