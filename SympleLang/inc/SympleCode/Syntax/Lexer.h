#pragma once

#include <string>

#include "SympleCode/Syntax/Token.h"

#include "SympleCode/DiagnosticBag.h"

namespace Symple::Syntax
{
	class Lexer
	{
	private:
		std::string mSource;
		unsigned mPosition = 0;
		shared_ptr<Lexer> mRef;

		char* mFile;
		unsigned mLine = 0, mColumn = 0;
		shared_ptr<DiagnosticBag> mDiagnosticBag = make_shared<DiagnosticBag>();
	public:
		Lexer(char* mFile);
		Lexer(char* mFile, std::string& mSource);
		void SetRef(shared_ptr<Lexer>); // Required before lexing

		shared_ptr<Token> Lex();

		shared_ptr<Token> LexAtom(Token::Kind);
		shared_ptr<Token> LexIdentifier();
		shared_ptr<Token> LexNumber();

		static bool IsWhiteSpace(char);
		static bool IsIdentifier(char);
		static bool IsInteger(char);
		static bool IsNumber(char);
		bool IsNumber();

		std::string_view GetSource();
		char* GetFile();

		shared_ptr<DiagnosticBag> GetDiagnosticBag();
	private:
		bool CheckNewLine();

		char& Peek(unsigned off = 0);
		char& Next();
	};
}