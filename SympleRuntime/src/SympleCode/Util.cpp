#include "SympleCode/Util.hpp"

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
}