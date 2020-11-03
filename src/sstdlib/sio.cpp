#include "sio.h"

namespace sstdlib
{
	void Print(const std::vector<sstring>& words)
	{
		if (words.size() > 2)
		{
			if (words[2][0] == '$')
			{
				sid vID = sGetVariebleID(words[2].substr(1, words[2].length()));
				switch (sGetVariebleClass(vID))
				{
				case BYTE_CLASS:
					printf("%i\n", sGetByteVarieble(vID));
					break;
				case SHORT_CLASS:
					printf("%i\n", sGetShortVarieble(vID));
					break;
				case INT_CLASS:
					printf("%i\n", sGetIntVarieble(vID));
					break;
				case LONG_CLASS:
					printf("%I64i\n", sGetLongVarieble(vID));
					break;
				case CHAR_CLASS:
					printf("%c\n", sGetCharVarieble(vID));
					break;
				case BOOL_CLASS:
					printf("%s\n", sGetBoolVarieble(vID) ? "true" : "false");
					break;
				}
			}
			else
				printf("%s\n", words[2].c_str());
		}
	}

	void Read(sid id)
	{
		switch (sGetVariebleClass(id))
		{
		case BYTE_CLASS:
		{
			sbyte val = (sbyte)0;
			scanf("%i", &val);

			sSetByteVarieble(id, val);
			break;
		}
		case SHORT_CLASS:
		{
			sshort val = (sshort)0;
			scanf("%i", &val);

			sSetShortVarieble(id, val);
			break;
		}
		case INT_CLASS:
		{
			sint val = (sint)0;
			scanf("%i", &val);

			sSetIntVarieble(id, val);
			break;
		}
		case LONG_CLASS:
		{
			slong val = (slong)0;
			scanf("%I64i", &val);

			sSetLongVarieble(id, val);
			break;
		}
		case CHAR_CLASS:
		{
			schar val = (schar)0;
			scanf("%c", &val);

			sSetCharVarieble(id, val);
			break;
		}
		case BOOL_CLASS:
		{
			char valstr[5];
			scanf("%s", valstr);

			sbool val = (sbool)0;
			if (valstr == "true" || valstr == "1")
				val = true;
			else if (valstr == "false" || valstr == "0")
				val = false;

			sSetBoolVarieble(id, val);
			break;
		}
		}
	}

	std::string ReadFile(const std::string& filepath)
	{
		FILE* fs = fopen(filepath.c_str(), "rb");

		fseek(fs, 0, SEEK_END);
		const int size = ftell(fs);
		const int allocaSize = size + 1;
		char* buff = (char*)_malloca(allocaSize);
		rewind(fs);

		if (buff == nullptr)
		{
			fclose(fs);
			return "";
		}

		fread(buff, sizeof(char), size, fs);
		fclose(fs);
		buff[size] = 0;

		return buff;
	}
}