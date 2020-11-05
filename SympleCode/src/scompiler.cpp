#include "scompiler.h"

namespace symple
{
	void CompileSource(const std::string& source)
	{
		std::vector<std::string> lines;

		{
			std::string s = source;
			size_t pos = 0;
			std::string tok;
			while ((pos = s.find(";")) != std::string::npos)
			{
				tok = s.substr(0, pos);

				lines.push_back(tok);

				s.erase(0, pos + std::string(";").length());
			}
			lines.push_back(s);
		}

		for (unsigned int i = 0; i < lines.size(); i++)
			CompileLine(lines[i]);
	}

	serializer CompileLine(const std::string& line)
	{
		serializer out;
		std::vector<std::string> words;

		{
			std::string s = line;
			size_t pos = 0;
			std::string tok;
			while ((pos = s.find(" ")) != std::string::npos)
			{
				tok = s.substr(0, pos);

				words.push_back(tok);

				s.erase(0, pos + std::string(" ").length());
			}
			words.push_back(s);
		}

		switch (std::hash<std::string>()(words[0]))
		{
		case VAR:
			out << CREATE_VAR;
			out << ParseInt(words[1]);
			break;
		case CALL:
			out << CALL_FUNC;
			out << std::hash<std::string>()(words[1]);
			break;
		default:
			printf("Called %X\n", std::hash<std::string>()(words[0]));
			break;
		}

		return out;
	}

	int ParseDigit(char c)
	{
		if (c < 48 || c > 57)
		{
			printf("Error Parsing Digit: '%c'\n", c);
			abort();
		}

		return c - 48;
	}

	int ParseInt(const std::string& value)
	{
		std::string svalue;
		bool negative;

		if (negative = value[0] == '-')
			svalue = value.substr(1);
		else
			svalue = value;

		int out = 0;
		for (unsigned int i = 0; i < svalue.length(); i++)
			out += ParseDigit(svalue[i]) * powl(10, (svalue.length() - i - 1));
		return negative ? -out : out;
	}
}