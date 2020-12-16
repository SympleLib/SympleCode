#include "SympleCode/Preprocessor/Preprocessor.h"

#include <iostream>
#include <sstream>

namespace Symple
{
	std::vector<const Token*> Preprocessor::Preprocess(const char* path)
	{
		std::string source = ReadFile(path);
		mLexer = source.c_str();

		const Token* current = new Token;
		while (!current->Is(Token::Kind::EndOfFile))
			mTokens.push_back(current = mLexer.Next());

		for (mCurrent = 0; mCurrent < mTokens.size(); mCurrent++)
		{
			if (Peek()->Is(Token::Kind::Comment))
			{
				mPreCurrent = 0;
				mLexer = (new std::string(Peek()->GetLex()))->c_str();
				mPreTokens.clear();
				current = new Token;
				while (!current->Is(Token::Kind::EndOfFile))
					mTokens.push_back(current = mLexer.Next());

				if (Peek()->Is(Token::Kind::Identifier))
				{
					std::string_view lex = PrePeek()->GetLex();

					if (lex == "include")
					{
						source = ReadFile(std::string(PreMatch(Token::Kind::String)->GetLex()).c_str());
						mLexer = source.c_str();
						while (!(current = mLexer.Next())->Is(Token::Kind::EndOfFile))
							mAllTokens.push_back(current);

						continue;
					}
				}
			}

			mAllTokens.push_back(Peek());
		}

		return mTokens;
	}

	const Token* Preprocessor::PreMatch(Token::Kind kind)
	{
		if (PrePeek()->Is(kind))
			return PreNext();
		PreNext();
		return new Token(kind);
	}

	const Token* Preprocessor::Peek(int offset)
	{
		unsigned int index = mCurrent + offset;
		if (index >= mTokens.size())
			return mTokens.back();
		return mTokens[index];
	}

	const Token* Preprocessor::PreNext()
	{
		return mPreTokens[mPreCurrent++];
	}

	const Token* Preprocessor::PrePeek(int offset)
	{
		unsigned int index = mPreCurrent + offset;
		if (index >= mPreTokens.size())
			return mPreTokens.back();
		return mPreTokens[index];
	}

	std::string Preprocessor::ReadFile(const char* path)
	{
		FILE* file;
		errno_t err;
		if (!(err = fopen_s(&file, path, "rb")) && file)
		{
			fseek(file, 0L, SEEK_END);
			unsigned int size = std::min(ftell(file), 4096L);
			rewind(file);
			char* source = new char[size + 1];
			fread(source, 1, size, file);
			source[size] = 0;
			fclose(file);

			return source;
		}

		char errMsg[32];
		if (!strerror_s(errMsg, err))
			std::cerr << "[!]: Error opening file '" << path << "': " << errMsg << "!\n";
		else
			std::cerr << "[!]: Unkown Error opening file '" << path << "'!\n";
		return {};
	}
}