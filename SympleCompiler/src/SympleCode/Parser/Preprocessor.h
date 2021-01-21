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

		const std::string mInclude;

		std::map<std::string, std::vector<const Token*>> mDefines;
	public:
		Preprocessor(const char* source = "", const char* file = "", const char* include = "", const std::vector<std::string>& includedFiles = {}, const std::map<std::string, std::vector<const Token*>>& defines = {});

		inline const auto& GetTokens() const
		{
			return mTokens;
		}
	private:
		void Preprocess(const Token* cmd);
	};
}