#pragma once

#include <map>
#include <vector>
#include <string>

#include "SympleCode/Parser/Lexer.h"

namespace Symple
{
	class Preprocessor
	{
	private:
		Lexer mLexer;
		std::vector<const Token*> mTokens;
		std::vector<const Token*> mAllTokens;
		std::vector<const Token*> mPreTokens;
		unsigned int mCurrent = 0, mPreCurrent = 0;

		std::map<std::string, std::string> mDefines;
	public:
		std::vector<const Token*> Preprocess(const char* path);
	private:
		const Token* Peek(int offset = 0);
		const Token* PreMatch(Token::Kind kind);
		const Token* PreNext();
		const Token* PrePeek(int offset = 0);

		std::string ReadFile(const char* path);
	};
}