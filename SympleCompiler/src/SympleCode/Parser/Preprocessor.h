#pragma once

#include <vector>
#include <map>

#include "SympleCode/Parser/Lexer.h"

namespace Symple
{
	class Preprocessor
	{
	private:
		Lexer mLexer;
		std::vector<const Token*> mTokens;
		std::vector<std::string> mIncludedFiles;

		std::map<std::string, std::vector<const Token*>> mDefines;
	public:
		Preprocessor(const char* source = "", const std::vector<std::string>& includedFiles = {}, const std::map<std::string, std::vector<const Token*>>& defines =
			{
				{ "null", { new Token(Token::Kind::Number, "0", 1) } },
				{ "ptr_size", { new Token(Token::Kind::Number, "4", 1) } }
			});

		inline const auto& GetTokens() const
		{
			return mTokens;
		}
	private:
		void Preprocess(const Token* cmd);
	};
}