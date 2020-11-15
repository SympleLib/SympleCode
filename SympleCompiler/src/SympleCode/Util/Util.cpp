#include "SympleCode/Util/Util.hpp"

namespace Symple
{
	std::vector<std::string> Split(std::string str, const std::string& del)
	{
		std::vector<std::string> out;

		size_t ppos = 0, pos;
		while ((pos = str.find(del)) != std::string::npos)
		{
			out.push_back(str.substr(0, pos));
			str.erase(0, pos + del.length());
		}
		out.push_back(str);

		return out;
	}

	void Write(const char* path, const char* text)
	{
		FILE* fs;
		if (fopen_s(&fs, path, "w") || fs == nullptr)
		{
			Err("Cannot Open File!");
			return;
		}
		fprintf(fs, text);
		fflush(fs);
		fclose(fs);
	}

	void Write(const std::string& path, const char* text) { Write(path.c_str(), text); }
	void Write(const std::string& path, const std::string& text) { Write(path.c_str(), text.c_str()); }

	int powi(int v, int t)
	{
		int val = 1;
		for (size_t i = 0; i < t; i++)
			val *= v;
		return val;
	}
}