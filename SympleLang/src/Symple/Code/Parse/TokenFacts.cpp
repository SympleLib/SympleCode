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

	NativeTypeKind TokenFacts::NativeType(TokenKind kind)
	{
		switch (kind)
		{
		case TokenKind::VoidKeyword:
			return NativeTypeKind::Void;
		case TokenKind::ByteKeyword:
			return NativeTypeKind::Byte;
		case TokenKind::ShortKeyword:
			return NativeTypeKind::Short;
		case TokenKind::IntKeyword:
			return NativeTypeKind::Int;
		case TokenKind::LongKeyword:
			return NativeTypeKind::Long;
		case TokenKind::FloatKeyword:
			return NativeTypeKind::Float;
		case TokenKind::DoubleKeyword:
			return NativeTypeKind::Double;
		case TokenKind::CharKeyword:
			return NativeTypeKind::Char;
		case TokenKind::WCharKeyword:
			return NativeTypeKind::WChar;
		case TokenKind::BoolKeyword:
			return NativeTypeKind::Bool;

		default:
			throw nullptr;
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