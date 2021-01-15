#pragma once

#include <string_view>
#include <initializer_list>

namespace Symple
{
	class Token
	{
	public:
		enum class Kind
		{
			Identifier,
			Number,
			String,
			Character,

			Equal,
			Exclamation,
			Plus,
			Minus,
			Asterisk,
			Slash,
			Percentage,

			PlusPlus,
			MinusMinus,

			EqualEqual,
			ExclamationEqual,
			PlusEqual,
			MinusEqual,
			AsteriskEqual,
			SlashEqual,
			PercentageEqual,

			LeftArrowEqual,
			RightArrowEqual,
			LeftArrowArrow,
			RightArrowArrow,

			Comma,
			Semicolon,
			Period,
			Colon,

			QuestionMark,

			OpenBracket,
			CloseBracket,
			OpenBrace,
			CloseBrace,
			OpenParenthesis,
			CloseParenthesis,

			LeftArrow,
			RightArrow,

			At,
			Pipe,
			PipePipe,
			PipeEqual,

			Ampersand,
			AmpersandAmpersand,
			AmpersandEqual,

			Asm,
			Null,
			Auto,

			True,
			False,
			While,
			For,
			Break,

			Hint,
			Return,
			Extern,
			Shared,
			Static,

			If,
			Else,
			Stalloc,

			SympleCall,
			StdCall,
			CCall,

			Mutable,
			Private,
			Signed,
			Unsigned,

			Struct,
			Enum,

			Comment,
			Preprocess,

			Last = Preprocess,

			Unknown = -2,
			EndOfFile = -1,
		};

		static constexpr const char* KindMap[(int)Token::Kind::Last + 1] = {
			"Identifier", "Number", "String", "Character",
			"Equal", "Exclamation", "Plus", "Minus", "Asterisk", "Slash", "Percentage",
			"PlusPlus", "MinusMinus",
			"EqualEqual", "ExclamationEqual", "PlusEqual", "MinusEqual", "AsteriskEqual", "SlashEqual", "PercentageEqual",
			"LeftArrowEqual", "RightArrowEqual", "LeftArrowArrow", "RightArrowArrow",
			"Comma", "Semicolon", "Period", "Colon",
			"QuestionMark",
			"OpenBrace", "CloseBrace", "OpenBracket", "CloseBracket", "OpenParenthesis", "CloseParenthesis",
			"LeftArrow", "RightArrow",
			"Asm", "Null", "Auto",
			"True", "False", "While", "For", "Break",
			"At", "Pipe", "PipePipe", "PipeEqual",
			"Ampersand", "AmpersandAmpersand", "AmpersandEqual",
			"Hint", "Return", "Extern", "Shared", "Static",
			"If", "Else", "Stalloc",
			"SympleCall", "StdCall", "CCall",
			"Mutable", "Private", "Signed", "Unsigned",
			"Struct", "Enum",
			"Comment", "Preprocess",
		};

		static const char* KindString(Kind kind);
	private:
		Kind mKind;
		std::string_view mLex;

		const char* mFile;
		int mLine, mColumn;
	public:
		static const Token* const Default;

		Token(Kind kind = Kind::Unknown, const char* file = "", int line = 0, int column = 0);
		Token(Kind kind, const char* beg, size_t len, const char* file = "", int line = 0, int column = 0);
		Token(Kind kind, const char* beg, const char* end, const char* file = "", int line = 0, int column = 0);

		bool Is(Kind kind) const;
		bool IsEither(std::initializer_list<Kind> kinds) const;

		const Kind& GetKind() const;
		const std::string_view& GetLex() const;
		const char* GetFile() const;
		int GetLine() const;
		int GetColumn() const;
	};
}