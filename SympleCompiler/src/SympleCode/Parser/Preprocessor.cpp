#include "SympleCode/Parser/Preprocessor.h"

#include <iostream>

namespace Symple
{
	Preprocessor::Preprocessor(const char* source, const std::vector<std::string>& includedFiles, const std::map<std::string, const Token*>& defines)
		: mLexer(source), mIncludedFiles(includedFiles), mDefines(defines)
	{
		const Token* current = Token::Default;
		while (!current->Is(Token::Kind::EndOfFile))
		{
			current = mLexer.Next();
			if (current->Is(Token::Kind::Preprocess))
				Preprocess(current);
			else
			{
				std::string lex(current->GetLex());
				if (mDefines.find(lex) == mDefines.end())
					mTokens.push_back(current);
				else
					mTokens.push_back(mDefines.at(lex));
			}
		}
	}

	void Preprocessor::Preprocess(const Token* cmd)
	{
		std::string* scmd = new std::string(cmd->GetLex());
		Lexer prepoLexer = scmd->c_str();
		Token* rcmd = prepoLexer.Next();

		if (rcmd->GetLex() == "include")
		{
			std::string includeDir(prepoLexer.Next()->GetLex());
			for (const std::string& dir : mIncludedFiles)
				if (includeDir == dir)
					return;
			mIncludedFiles.push_back(includeDir);

			FILE* file;
			errno_t err;
			if (!(err = fopen_s(&file, ("sy\\" + includeDir).c_str(), "rb")) && file)
			{
				fseek(file, 0L, SEEK_END);
				unsigned int size = std::min(ftell(file), 4096L);
				rewind(file);
				char* source = new char[size + 1];
				fread(source, 1, size, file);
				source[size] = 0;
				fclose(file);

				Preprocessor includePreprocessor(source, mIncludedFiles, mDefines);
				mDefines = includePreprocessor.mDefines;
				for (const Token* token : includePreprocessor.GetTokens())
					if (token != includePreprocessor.GetTokens().back())
					{
						std::string lex(token->GetLex());
						if (mDefines.find(lex) == mDefines.end())
							mTokens.push_back(token);
						else
							mTokens.push_back(mDefines.at(lex));
					}
			}
		}
		else if (rcmd->GetLex() == "define")
		{
			std::string replace(prepoLexer.Next()->GetLex());
			Token* with = prepoLexer.Next();

			mDefines.insert({ replace, with });
		}
	}
}