#include "sparser.h"

#include <sdebug.h>

#include <svm.h>
#include <scm.h>

#include "sserializer.h"

namespace scp
{
	serializer ss;
	sstring file;
	suint line;

	std::vector<sstring> lines;

	void ParseFile(const sstring& filepath)
	{
		FILE* fs;
		fopen_s(&fs, filepath.c_str(), "rb");

		fseek(fs, 0, SEEK_END);
		const int size = ftell(fs);
		const int allocaSize = size + 1;
		char* buff = (char*)alloca(allocaSize);
		rewind(fs);

		fread(buff, sizeof(char), size, fs);
		fclose(fs);
		buff[size] = 0;

		ParseSource(buff);
	}

	void ParseSource(const sstring& source)
	{
		ss = serializer();

		{
			lines.clear();
			sstring s = source;
			size_t pos = 0;
			sstring tok;
			while ((pos = s.find("\r\n")) != sstring::npos)
			{
				tok = s.substr(0, pos);

				lines.push_back(tok);

				s.erase(0, pos + sstring("\r\n").length());
			}
			lines.push_back(s);
		}

		for (line = 0; line < lines.size(); line++)
		{
			ParseLine(lines[line]);
		}
	}

	void ParseLine(const sstring& mline)
	{
		std::vector<sstring> words;

		{
			sstring s = mline;
			size_t pos = 0;
			sstring tok;
			while ((pos = s.find(";")) != sstring::npos)
			{
				tok = s.substr(0, pos);

				words.push_back(tok);

				s.erase(0, pos + sstring(";").length());
			}
			lines.push_back(s);
		}

		printf("%s\n", mline.c_str());

		if (words.size() > 1)
		{
			for (const auto& item : scm::GetClassNames())
			{
				if (words[0] == item.first)
				{
					sclassid id = item.second;
					stypeid type = scm::GetClassType(id);
					
					svm::CreateVarieble(words[1], type, id);
				}
			}
		}
	}
}