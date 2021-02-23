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

#define PUNC(str, ty) \
	else if (!strncmp(Current, str, strlen(str))) \
	{ \
		char *beg = Current; \
		for (unsigned i = 0; i < strlen(str); i++) \
			Next(); \
		return make_shared<Token>(Token::##ty, beg, &Next(), mTrivia, mLine, mColumn, mFile); \
	}

	shared_ptr<Token> Lexer::Lex()
	{
		unsigned trKind = mPosition ? Trivia::Unknown : Trivia::StartOfLine;
		unsigned trPosition = mPosition;
		unsigned trLn = mLine;
		unsigned trCl = mColumn;

		bool checkNewLine;
		while ((checkNewLine = CheckNewLine()) || IsWhiteSpace(Peek()))
		{
			if (checkNewLine)
				trKind |= Trivia::StartOfLine;
			else
				trKind |= Trivia::LeadingSpace;

			Next();
		}

		mTrivia = make_shared<Trivia>(trKind, &mSource[trPosition], Current, trLn, trCl, mFile);

		char c = Peek();
		if (!c)
			return LexAtom(Token::EndOfFile);

		if (IsNumber())
			return LexNumber();
		else if (IsIdentifier(c))
			return LexIdentifier();

		PUNC("=>", EqualArrow)

		else
			switch (c)
			{
			ATOM(',', Comma);
			ATOM(':', Colon);
			ATOM('.', Period);
			ATOM(';', Semicolon);

			ATOM('+', Plus);
			ATOM('-', Dash);
			ATOM('!', Exclamation);
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
	{ return make_shared<Token>(kind, &Next(), 1, mTrivia, mLine, mColumn, mFile); }

#define KEYWORD(word, key) \
	else if (text == #word) \
		return make_shared<Token>(Token::##key##Keyword, text, mTrivia, mLine, column, mFile)

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

		KEYWORD(static, Static);
		KEYWORD(local, Local);
		KEYWORD(private, Local);
		KEYWORD(globl, Global);
		KEYWORD(global, Global);
		KEYWORD(public, Global);

		KEYWORD(__asm, Native);
		KEYWORD(native, Native);
		KEYWORD(goto, Goto);

		KEYWORD(if, If);
		KEYWORD(else, Else);

		KEYWORD(struct, Struct);
		KEYWORD(structure, Struct);

		else
			return make_shared<Token>(Token::Identifier, text, mTrivia, mLine, column, mFile);
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

		return make_shared<Token>(Token::String, beg, end, mTrivia, ln, column, mFile);
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
			return make_shared<Token>(Token::Float, beg, &Next(), mTrivia, mLine, column, mFile);
		else if (dotCount)
			return make_shared<Token>(Token::Number, beg, Current, mTrivia, mLine, column, mFile);
		else
			return make_shared<Token>(Token::Integer, beg, Current, mTrivia, mLine, column, mFile);
	}
}