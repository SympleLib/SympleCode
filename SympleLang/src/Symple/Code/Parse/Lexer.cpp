#include "Symple/Code/Parse/Lexer.h"

namespace Symple::Code
{
	Lexer::Lexer(const GlobalRef<const File> &file)
		: m_File(file), m_Pointer(file->Source.c_str()) {}

	TokenList Lexer::LexAll()
	{
		TokenList toks;

		do
			toks.push_back(Lex());
		while (Current);
		toks.push_back(Lex());
		return toks;
	}

	GlobalRef<Token> Lexer::Lex()
	{
		while (true)
		{
			if (Current == '\n')
			{
				m_Line++;
				m_DisplayLine++;
				m_Column = 1;
				m_Pointer++;
			}
			else if (!strncmp(&Current, "//", 2))
			{
				Next();
				Next();
				while (Current != '\n')
					Next();
			}
			else if (!strncmp(&Current, "/*", 2))
			{
				Next();
				Next();
				while (strncmp(&Current, "*/", 2))
					Next();
				Next();
				Next();
			}
			else if (IsWhiteSpace(Current))
				Next();
			else
				break;
		}

		if (!Current)
		{
			const char *beg = &Next();
			return MakeToken(TokenKind::EndOfFile, beg, &Current);
		}

		if (IsNumber(Current))
			return LexNumber();
		else if (IsIdentifier(Current))
			return LexIdentifier();
		else if (Current == '`')
		{
			Next();
			const char *beg = &Next();
			assert(Current == '`');
			const char *end = &Next();

			return MakeToken(TokenKind::Char, beg, end);
		}
		else if (Current == '\'')
		{
			Next();
			const char *beg = &Next();
			while (Current != '\'')
				Next();
			const char *end = &Next();

			return MakeToken(TokenKind::Identifier, beg, end);
		}
		else if (Current == '"')
		{
			Next();
			const char *beg = &Next();
			while (Current != '"')
				Next();
			const char *end = &Next();

			return MakeToken(TokenKind::String, beg, end);
		}
		else
			return LexPunctuation();
	}

	GlobalRef<Token> Lexer::LexNumber()
	{
		const char *beg = &Next();
		while (IsNumber(Current))
			Next();
		return MakeToken(TokenKind::Number, beg, &Current);
	}
	
	#define KEYWORD(key, word) if (identifier == #key) return MakeToken(TokenKind::##word##Keyword, beg, &Current)
	#define ID_KEYWORD(key, word) if (identifier == #key) return MakeToken(TokenKind::##word##Keyword, beg, &Current, true)
	GlobalRef<Token> Lexer::LexIdentifier()
	{
		const char *beg = &Next();
		while (IsIdentifier(Current))
			Next();

		std::string_view identifier(beg, std::distance(beg, &Current));
		KEYWORD(void, Void);
		KEYWORD(byte, Byte);
		KEYWORD(short, Short);
		KEYWORD(int, Int);
		KEYWORD(long, Long);

		KEYWORD(float, Float);
		KEYWORD(double, Double);
		KEYWORD(triple, Triple);

		KEYWORD(char, Char);
		KEYWORD(wchar, WChar);
		KEYWORD(_Bool, Bool);
		KEYWORD(bool, Bool);

		ID_KEYWORD(ret, Return);
		KEYWORD(return, Return);

		KEYWORD(__extern, Extern);
		ID_KEYWORD(extern, Extern);
		ID_KEYWORD(external, Extern);

		ID_KEYWORD(ccall, CCall);
		ID_KEYWORD(stdcall, StdCall);
		ID_KEYWORD(sycall, SyCall);

		KEYWORD(__cdecl, CCall);
		KEYWORD(__ccall, CCall);
		KEYWORD(__stdcall, StdCall);
		KEYWORD(__sycall, SyCall);

		ID_KEYWORD(stalloc, Stalloc);
		ID_KEYWORD(staf, Staf);

		KEYWORD(__stalloc, Stalloc);
		KEYWORD(__staf, Staf);

		KEYWORD(__struct, Struct);
		KEYWORD(__structure, Struct);
		ID_KEYWORD(struct, Struct);
		ID_KEYWORD(structure, Struct);

		KEYWORD(__lengthof, Lengthof);
		ID_KEYWORD(lengthof, Lengthof);
		KEYWORD(__sizeof, Lengthof);
		ID_KEYWORD(sizeof, Sizeof);
		
		return MakeToken(TokenKind::Identifier, beg, &Current);
	}
	#undef KEYWORD

	GlobalRef<Token> Lexer::LexPunctuation()
	{
		const char *beg = &Current;

		for (uint32 i = sizeof(s_Punctuators) / sizeof(*s_Punctuators); i; i--)
			if (!strncmp(&Current, s_Punctuators[i - 1], strlen(s_Punctuators[i - 1])))
			{
				for (uint32 j = 0; j < strlen(s_Punctuators[i - 1]); j++)
					Next();
				return MakeToken((TokenKind)((uint32)TokenKind::Punctuator + i - 1), beg, &Current);
			}

		Next();
		return MakeToken(TokenKind::Unknown, beg, &Current);
	}


	bool Lexer::IsNumber(char c)
	{ return c >= '0' && c <= '9' || c == '.'; }

	bool Lexer::IsIdentifier(char c)
	{ return c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z' || c == '_' || c >= '0' && c <= '9'; }

	bool Lexer::IsWhiteSpace(char c)
	{ return c == ' ' || c == '\t' || c == '\r'; }


	const char &Lexer::Peek()
	{ return *m_Pointer; }

	const char &Lexer::Next()
	{
		const char &prev = Current;
		m_Column++;
		m_Pointer++;
		return prev;
	}


	GlobalRef<Token> Lexer::MakeToken(TokenKind kind, const char *beg, const char *end, bool isIdKey)
	{ return MakeRef<Token>(kind, beg, end, m_File, m_Line, m_DisplayLine, m_Column, isIdKey); }
}