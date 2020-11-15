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

	void Write(const char* path, const char* data)
	{
		FILE* fs;
		if (fopen_s(&fs, path, "w") || fs == nullptr)
		{
			Err("Cannot Open File!");
			return;
		}
		fprintf(fs, data);
		fflush(fs);
		fclose(fs);
	}

	int powi(int v, int t)
	{
		int val = 1;
		for (size_t i = 0; i < t; i++)
			val *= v;
		return val;
	}
}