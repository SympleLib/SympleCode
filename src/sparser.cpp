#include "sparser.h"

#include <vector>

#include "sstdlib/sstdlib.h"

unsigned int line;
std::string file;
bool comented;

void sParseFile(const sstring& filepath)
{
	file = filepath;

	sParseSource(sstdlib::ReadFile(filepath));
}

void sParseSource(const sstring& source)
{
	std::vector<std::string> lines;

	{
		std::string s = source;
		size_t pos = 0;
		std::string tok;
		while ((pos = s.find("\r\n")) != std::string::npos)
		{
			tok = s.substr(0, pos);

			lines.push_back(tok);

			s.erase(0, pos + std::string("\r\n").length());
		}
		lines.push_back(s);
	}

	for (unsigned int i = 0; i < lines.size(); i++)
	{
		sPreParseLine(lines[i]);
	}

	for (line = 0; line < lines.size(); line++)
	{
		sParseLine(lines[line]);
	}
}

void sPreParseLine(sstring& mline)
{
	if (mline.substr(0, 2) == "/#")
	{
		comented = !comented;
		mline = "";
	}
	if (comented || mline[0] == '#')
		mline = "";
}

void sParseLine(const sstring& mline)
{
	std::vector<std::string> words;

	{
		std::string s = mline;
		size_t pos = 0;
		std::string tok;
		while ((pos = s.find(" ")) != std::string::npos)
		{
			tok = s.substr(0, pos);

			words.push_back(tok);

			s.erase(0, pos + std::string(" ").length());
		}
		words.push_back(s.substr(0, s.length()));
		s.clear();
	}

	if (words.size() > 0)
	{
		if (words[0] == "var")
		{
			if (words.size() > 1)
			{
				const sstring& varName = words[1];

				if (words.size() > 2)
				{
					if (words[2] == "byte")
					{
						sid id;
						if (sVariebleExits(varName))
							id = sGetVariebleID(varName);
						else
							id = sGenByteVarieble(varName);

						if (words.size() > 3)
						{
							sbyte value = sParseByte(words[3]);
							sSetByteVarieble(id, value);
						}
					}
					else if (words[2] == "short")
					{
						sid id;
						if (sVariebleExits(varName))
							id = sGetVariebleID(varName);
						else
							id = sGenShortVarieble(varName);

						if (words.size() > 3)
						{
							sshort value = sParseShort(words[3]);
							sSetShortVarieble(id, value);
						}
					}
					else if (words[2] == "int")
					{
						sid id;
						if (sVariebleExits(varName))
							id = sGetVariebleID(varName);
						else
							id = sGenIntVarieble(varName);

						if (words.size() > 3)
						{
							sint value = sParseInt(words[3]);
							sSetIntVarieble(id, value);
						}
					}
					else if (words[2] == "long")
					{
						sid id;
						if (sVariebleExits(varName))
							id = sGetVariebleID(varName);
						else
							id = sGenLongVarieble(varName);

						if (words.size() > 3)
						{
							slong value = sParseLong(words[3]);
							sSetLongVarieble(id, value);
						}
					}
					else if (words[2] == "char")
					{
						sid id;
						if (sVariebleExits(varName))
							id = sGetVariebleID(varName);
						else
							id = sGenCharVarieble(varName);

						if (words.size() > 3)
						{
							schar value = sParseChar(words[3]);
							sSetCharVarieble(id, value);
						}
					}
					else if (words[2] == "bool")
					{
						sid id;
						if (sVariebleExits(varName))
							id = sGetVariebleID(varName);
						else
							id = sGenBoolVarieble(varName);

						if (words.size() > 3)
						{
							sbool value = sParseBool(words[3]);
							sSetBoolVarieble(id, value);
						}
					}
				}
			}
		}
		else if (words[0] == "delete")
		{
			if (words.size() > 1)
			{
				const sstring& varName = words[1];

				if (words.size() > 2)
				{
					if (words[2] == "byte")
					{
						sDelByteVarieble(varName);
					}
					else if (words[2] == "short")
					{
						sDelShortVarieble(varName);
					}
					else if (words[2] == "int")
					{
						sDelIntVarieble(varName);
					}
					else if (words[2] == "long")
					{
						sDelLongVarieble(varName);
					}
					else if (words[2] == "char")
					{
						sDelCharVarieble(varName);
					}
					else if (words[2] == "bool")
					{
						sDelBoolVarieble(varName);
					}
				}
			}
		}
		else if (words[0] == "set")
		{
			if (words.size() > 1)
			{
				const sstring& varName = words[1];
				sid id = sGetVariebleID(varName);

				if (words.size() > 2)
				{
					switch (sGetVariebleClass(id))
					{
					case BYTE_CLASS:
					{
						sbyte value = sParseByte(words[2]);
						sSetByteVarieble(id, value);
						break;
					}
					case SHORT_CLASS:
					{
						sshort value = sParseShort(words[2]);
						sSetShortVarieble(id, value);
						break;
					}
					case INT_CLASS:
					{
						sint value = sParseInt(words[2]);
						sSetIntVarieble(id, value);
						break;
					}
					case LONG_CLASS:
					{
						slong value = sParseLong(words[2]);
						sSetLongVarieble(id, value);
						break;
					}
					case CHAR_CLASS:
					{
						schar value = sParseChar(words[2]);
						sSetCharVarieble(id, value);
						break;
					}
					case BOOL_CLASS:
					{
						sbool value = sParseByte(words[2]);
						sSetBoolVarieble(id, value);
						break;
					}
					}
				}
			}
		}
		else if (words[0] == "function")
		{
			
		}
		else if (words[0] == "call")
		{
			if (words.size() > 1)
			{
				if (words[1] == "print")
				{
					sstdlib::Print(words);
				}
				else if (words[1] == "read" && words.size() > 2)
				{
					sstdlib::Read(sGetVariebleID(words[2]));
				}
			}
		}
		else if (words[0] == "goto")
		{
			if (words.size() > 1)
				line = sParseInt(words[1]);
		}
	}
}

#pragma region Parsing Values
sint sParseDigit(char c)
{
	if (c < 48 || c > 57)
	{
		printf("%c\n", c);
		SERROR("Character not a digit!", "test.smpl", line);
	}

	return c - 48;
}

sbyte sParseByte(const sstring& str)
{
	if (str[0] == '$')
	{
		return sstdlib::CastToByte(sGetVariebleID(str.substr(1, str.length())));
	}
	else
	{
		sbyte out = 0;
		for (unsigned int i = 0; i < str.length(); i++)
			out += sParseDigit(str[i]) * powl(10, (str.length() - i - 1));
		return (sbyte)out;
	}
}

sshort sParseShort(const sstring& str)
{
	if (str[0] == '$')
	{
		return sstdlib::CastToShort(sGetVariebleID(str.substr(1, str.length())));
	}
	else
	{
		sshort out = 0;
		for (unsigned int i = 0; i < str.length(); i++)
			out += sParseDigit(str[i]) * powl(10, (str.length() - i - 1));
		return (sshort)out;
	}
}

sint sParseInt(const sstring& str)
{

	if (str[0] == '$')
	{
		return sstdlib::CastToInt(sGetVariebleID(str.substr(1, str.length())));
	}
	else
	{
		sint out = 0;
		for (unsigned int i = 0; i < str.length(); i++)
		{
			out += sParseDigit(str[i]) * powl(10, (str.length() - i - 1));
		}
		return (sint)out;
	}
}

slong sParseLong(const sstring& str)
{

	if (str[0] == '$')
	{
		return sstdlib::CastToLong(sGetVariebleID(str.substr(1, str.length())));
	}
	else
	{
		slong out = 0;
		for (unsigned int i = 0; i < str.length(); i++)
			out += (slong)sParseDigit(str[i]) * powl(10, (str.length() - i - 1));
		return (slong)out;
	}
}

schar sParseChar(const sstring& str)
{
	if (str[0] == '\'' && str[2] == '\'')
	{
		return str[1];
	}
	else 
	{
		if (str[0] == '$')
		{
			return sstdlib::CastToChar(sGetVariebleID(str.substr(1, str.length())));
		}
		else
		{
			printf("parsing digit: '%s'\n", str.c_str());

			schar out = 0;
			for (unsigned int i = 0; i < str.length(); i++)
				out += sParseDigit(str[i]) * powl(10, (str.length() - i - 1));
			return (schar)out;
		}
	}
}

sbool sParseBool(const sstring& str)
{
	if (str[0] == '$')
	{
		return sstdlib::CastToBool(sGetVariebleID(str.substr(1, str.length())));
	}
	else
	{
		if (str == "true")
			return true;
		else if (str == "false")
			return false;
		else if (str == "1")
			return true;
		else if (str == "0")
			return false;
		SERROR("Unable to parse bool!", str.c_str(), -1);
	}
}
#pragma endregion

unsigned int sGetCurrentLine()
{
	return line;
}


std::string sGetCurrentFile()
{
	return file;
}