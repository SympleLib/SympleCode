#include "SympleCode/Syntax/Lexer.h"

#include "SympleCode/Util/FileUtil.h"

#define Current &Peek()

namespace Symple::Syntax
{
	Lexer::Lexer(char* file)
		: mFile(file)
	{
		FILE* fs = Util::OpenFile(file, "rb");
		if (fs)
		{
			mSource = Util::ReadFile(fs);
			Util::CloseFile(fs);
		}
	}

	Lexer::Lexer(char* file, std::string& source)
		: mFile(file), mSource(source)
	{}


#define ATOM(char, ty) \
	case char: \
		return LexAtom(Token::##ty)

	shared_ptr<Token> Lexer::Lex()
	{
		while (CheckNewLine() || IsWhiteSpace(Peek()))
			Next();

		char c = Peek();
		if (!c)
			return LexAtom(Token::EndOfFile);

		if (IsNumber())
			return LexNumber();
		if (IsIdentifier(c))
			return LexIdentifier();

		switch (c)
		{
		ATOM(',', Comma);
		ATOM(':', Colon);
		ATOM(';', Semicolon);

		ATOM('+', Plus);
		ATOM('-', Dash);
		ATOM('*', Asterisk);
		ATOM('/', Slash);
		ATOM('%', Percentage);

		ATOM('=', Equal);

		ATOM('(', OpenParenthesis);
		ATOM(')', CloseParenthesis);
		ATOM('{', OpenBrace);
		ATOM('}', CloseBrace);

		case '"':
			return LexString();

		default:
			auto tok = LexAtom(Token::Unknown);
			mDiagnosticBag->ReportUnknownToken(tok);
			return tok;
		}
	}


	std::string_view Lexer::GetSource()
	{ return mSource; }

	char* Lexer::GetFile()
	{ return mFile; }


	shared_ptr<DiagnosticBag> Lexer::GetDiagnosticBag()
	{ return mDiagnosticBag; }


	bool Lexer::IsWhiteSpace(char c)
	{ return c == ' ' || c == '\t' || c == '\r' || c == 0xCC; }

	bool Lexer::IsIdentifier(char c)
	{ return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || c == '$' || (c >= '0' && c <= '9'); }

	bool Lexer::IsInteger(char c)
	{ return c >= '0' && c <= '9'; }

	bool Lexer::IsNumber(char c)
	{ return IsInteger(c) || c == '.'; }

	bool Lexer::IsNumber()
	{
		if (IsInteger(Peek()))
			return true;
		else if (Peek() == '.' && IsInteger(Peek(1)))
			return true;
		else
			return false;
	}

	bool Lexer::CheckNewLine()
	{
		if (Peek() == '\n')
		{
			mLine++;
			mColumn = 0;

			return true;
		}
		else
			return false;
	}


	char& Lexer::Peek(unsigned off)
	{
		unsigned pos = mPosition + off;
		if (pos >= mSource.length())
			return mSource[mSource.length()];
		return mSource[pos];
	}

	char& Lexer::Next()
	{
		char& prev = mSource[mPosition];
		mColumn++;
		mPosition++;
		if (mPosition > mSource.length())
		{
			mColumn--;
			mPosition--;
		}
		return prev;
	}


	shared_ptr<Token> Lexer::LexAtom(Token::Kind kind)
	{
		return make_shared<Token>(kind, &Next(), 1, mLine, mColumn, mFile);
	}

#define KEYWORD(word, key) \
	else if (text == #word) \
		return make_shared<Token>(Token::##key##Keyword, text, mLine, column, mFile)

	shared_ptr<Token> Lexer::LexIdentifier()
	{
		char* beg = Current;
		unsigned column = mColumn;
		Next();
		while (IsIdentifier(Peek()))
			Next();

		std::string_view text(beg, std::distance(beg, Current));
		if (false);

		KEYWORD(void, Void);
		KEYWORD(byte, Byte);
		KEYWORD(short, Short);
		KEYWORD(int, Int);
		KEYWORD(integer, Int);
		KEYWORD(long, Long);

		KEYWORD(bool, Bool);
		KEYWORD(boolean, Bool);
		KEYWORD(char, Char);
		KEYWORD(wchar, WChar);
		KEYWORD(wchar_t, WChar);

		KEYWORD(float, Float);
		KEYWORD(double, Double);
		KEYWORD(triple, Triple);

		KEYWORD(ret, Return);
		KEYWORD(return, Return);

		KEYWORD(default, Default);
		KEYWORD(extern, Extern);
		KEYWORD(external, Extern);

		KEYWORD(link, Import);
		KEYWORD(import, Import);
		KEYWORD(cdecl, CDecl);
		KEYWORD(__cdecl, CDecl);
		KEYWORD(stdcall, StdCall);
		KEYWORD(__stdcall, StdCall);

		KEYWORD(dllimport, DllImport);
		KEYWORD(__dllimport, DllImport);
		KEYWORD(dllexport, DllExport);
		KEYWORD(__dllexport, DllExport);

		KEYWORD(__asm, Native);
		KEYWORD(native, Native);
		KEYWORD(goto, Goto);

		else
			return make_shared<Token>(Token::Identifier, text, mLine, column, mFile);
	}

	shared_ptr<Token> Lexer::LexString()
	{
		Next(); // Eat "
		char* beg = Current;
		unsigned ln = mLine;
		unsigned column = mColumn;
		while (Peek() != '"')
			Next();
		char* end = Current;
		Next();

		return make_shared<Token>(Token::String, beg, end, ln, column, mFile);
	}

	shared_ptr<Token> Lexer::LexNumber()
	{
		unsigned dotCount = 0;

		char* beg = Current;
		unsigned column = mColumn;
		if (!IsInteger(Next()))
			dotCount++;
		while (IsInteger(Peek()) || (IsNumber(Peek()) && ++dotCount))
			Next();

		if (Peek() == 'f' || Peek() == 'F')
			return make_shared<Token>(Token::Float, beg, &Next(), mLine, column, mFile);
		else if (dotCount)
			return make_shared<Token>(Token::Number, beg, Current, mLine, column, mFile);
		else
			return make_shared<Token>(Token::Integer, beg, Current, mLine, column, mFile);
	}
}