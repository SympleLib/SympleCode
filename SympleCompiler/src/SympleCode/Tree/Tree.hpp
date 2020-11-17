#pragma once

#include <vector>
#include <string>
#include <ostream>
#include <any>

typedef struct Branch
{
	typedef int8_t byte;
	typedef size_t size;
#if WIN32
	typedef int32_t integer;
#else
	typedef int64_t integer;
#endif
	typedef std::string string;
	typedef std::string_view string_view;
	typedef const char* cstring;

	std::vector<Branch> SubBranches;
	std::string Label;
	std::any Data;

	Branch();

	Branch(const std::string& label);

	Branch(const std::string& label, const std::any& data);

	Branch& PushBranch(const Branch& branch);
	Branch& PushBranch(const std::string& label);
	Branch& PushBranch(const std::string& label, const std::any& data);

	Branch& FindBranch(const std::string& label);
	const Branch& FindBranch(const std::string& label) const;

	string ToString() const;

	operator string() const;

	inline friend std::ostream& operator <<(std::ostream& os, const Branch& br)
	{
		return os << br.ToString();
	}
private:
	string ThisString(unsigned int tabs) const;
} Tree;