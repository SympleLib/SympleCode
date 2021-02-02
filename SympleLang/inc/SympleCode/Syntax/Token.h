#pragma once

#include <iostream>
#include <string_view>

#include "SympleCode/Memory.h"

namespace Symple::Syntax
{
	class Lexer;

	class Token
	{
	public: enum Kind : unsigned;
	private:
		Kind mKind;
		std::string_view mText;
		shared_ptr<Lexer> mLexer; // Reference to Lexer (has source text)

		char* mFile;
		unsigned mLine, mColumn;
	public:
		Token(Kind = Unknown, unsigned ln = 0, unsigned col = 0, char* file = "<NA>", shared_ptr<Lexer> = nullptr);
		Token(Kind, std::string_view text, unsigned ln = 0, unsigned col = 0, char* file = "<NA>", shared_ptr<Lexer> = nullptr);
		Token(Kind, char* beg, unsigned len = 1, unsigned ln = 0, unsigned col = 0, char* file = "<NA>", shared_ptr<Lexer> = nullptr);
		Token(Kind, char* beg, char* end, unsigned ln = 0, unsigned col = 0, char* file = "<NA>", shared_ptr<Lexer> = nullptr);

		bool Is(Kind kind);
		template <typename... Args>
		bool Is(Kind kind, Args... kinds)
		{ return Is(kind) || Is(kinds...); }

		void Print(std::ostream& = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "");
		void PrintShort(std::ostream& = std::cout);

		Kind GetKind();
		std::string_view GetText();
		shared_ptr<Lexer> GetLexer();

		char* GetFile();
		unsigned GetLine();
		unsigned GetColumn();

		static shared_ptr<Token> Error;
		static shared_ptr<Token> Default;
	public:
		enum Kind : unsigned
		{
			EndOfFile,
			Unknown,

			Identifier,
			Integer,
			Number,
			Float,

			Comma,
			Semicolon,

			Plus,
			Dash,
			Asterisk,
			Slash,
			Percentage,

			Equal,

			OpenParenthesis,
			CloseParenthesis,
			OpenBrace,
			CloseBrace,


			VoidKeyword,
			ByteKeyword,
			ShortKeyword,
			IntKeyword,
			LongKeyword,

			BoolKeyword,
			CharKeyword,
			WCharKeyword,

			FloatKeyword,
			DoubleKeyword,
			TripleKeyword,

			Last = TripleKeyword,
		};

		static constexpr char* KindMap[Last + 1] = {
			"EndOfFile",
			"Unknown",

			"Identifier",
			"Integer",
			"Number",
			"Float",

			"Comma",
			"Semicolon",

			"Plus",
			"Dash",
			"Asterisk",
			"Slash",
			"Percentage",

			"Equal",

			"OpenParenthesis",
			"CloseParenthesis",
			"OpenBrace",
			"CloseBrace",


			"VoidKeyword",
			"ByteKeyword",
			"ShortKeyword",
			"IntKeyword",
			"LongKeyword",

			"BoolKeyword",
			"CharKeyword",
			"WCharKeyword",

			"FloatKeyword",
			"DoubleKeyword",
			"TripleKeyword",
		};
	};
}