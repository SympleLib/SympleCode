#pragma once

#include <unordered_map>
#include <string>
#include <any>

struct Branch
{
	typedef int8_t byte;
	typedef size_t size;
#if WIN32
	typedef int32_t integer;
#else
	typedef int64_t integer;
#endif
	typedef std::string string;

	std::unordered_map<std::string, Branch> SubBranches;
	std::string Label;
	std::any Data;

	Branch() {}

	Branch(const std::string& label)
		: Label(label) {}

	Branch& PushBranch(const std::string& label);

	string ToString() const;

	operator string() const;

	inline friend std::ostream& operator <<(std::ostream& os, const Branch& br)
	{
		os << br.ToString();
		return os;
	}
private:
	string ThisString(unsigned int tabs) const;
};