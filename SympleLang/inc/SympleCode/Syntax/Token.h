#pragma once

#include "SympleCode/Syntax/Trivia.h"

namespace Symple::Syntax
{
	class __SYC_API Token
	{
	public: enum Kind : unsigned;
	private:
		Kind mKind;
		std::string mText;
		shared_ptr<Trivia> mTrivia;

		char* mFile;
		unsigned mLine, mColumn;
	public:
		Token(Kind = Unknown, shared_ptr<Trivia> = Trivia::Default, unsigned ln = 0, unsigned col = 0, char* file = "<NA>");
		Token(Kind, std::string_view text, shared_ptr<Trivia> = Trivia::Default, unsigned ln = 0, unsigned col = 0, char* file = "<NA>");
		Token(Kind, char* beg, unsigned len = 1, shared_ptr<Trivia> = Trivia::Default, unsigned ln = 0, unsigned col = 0, char* file = "<NA>");
		Token(Kind, char* beg, char* end, shared_ptr<Trivia> = Trivia::Default, unsigned ln = 0, unsigned col = 0, char* file = "<NA>");

		bool IsKeyword();
		bool Is(Kind kind);
		template <typename... Args>
		bool Is(Kind kind, Args... kinds)
		{ return Is(kind) || Is(kinds...); }

		void Print(std::ostream& = std::cout, std::string_view indent = "", bool last = true, std::string_view label = "");
		void PrintShort(std::ostream& = std::cout);

		Kind GetKind();
		std::string_view GetText();
		shared_ptr<Trivia> GetTrivia();

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
			String,
			Integer,
			Number,
			Float,

			Comma,
			Colon,
			Semicolon,

			Plus,
			Dash,
			Asterisk,
			Slash,
			Percentage,

			Exclamation,

			Equal,
			EqualArrow,

			OpenParenthesis,
			CloseParenthesis,
			OpenBrace,
			CloseBrace,

			FirstKeyword,
			VoidKeyword = FirstKeyword,
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

			ReturnKeyword,
			DefaultKeyword,

			ExternKeyword,
			ImportKeyword,

			CDeclKeyword,
			StdCallKeyword,
			DllImportKeyword,
			DllExportKeyword,

			StaticKeyword,
			LocalKeyword,
			GlobalKeyword,

			NativeKeyword,
			GotoKeyword,

			IfKeyword,
			ElseKeyword,

			StructKeyword,

			Last = StructKeyword,
		};

		static constexpr char* KindMap[Last + 1] = {
			"EndOfFile",
			"Unknown",

			"Identifier",
			"String",
			"Integer",
			"Number",
			"Float",

			"Comma",
			"Colon",
			"Semicolon",

			"Plus",
			"Dash",
			"Asterisk",
			"Slash",
			"Percentage",

			"Exclamation",

			"Equal",
			"EqualArrow",

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

			"ReturnKeyword",
			"DefaultKeyword",

			"ExternKeyword",
			"ImportKeyword",

			"CDeclKeyword",
			"StdCallKeyword",
			"DllImportKeyword",
			"DllExportKeyword",

			"StaticKeyword",
			"LocalKeyword",
			"GlobalKeyword",

			"NativeKeyword",
			"GotoKeyword",

			"IfKeyword",
			"ElseKeyword",

			"StructKeyword",
		};
	};

	typedef std::vector<shared_ptr<Token>> TokenList;
}