#include "SympleCode/Tree/Tree.hpp"

#include <sstream>

Branch& Branch::PushBranch(const std::string& label)
{
	SubBranches.push_back({ label });
	return SubBranches.at(SubBranches.size() - 1);
}

Branch& Branch::PushBranch(const std::string& label, const std::any& data)
{
	SubBranches.push_back({ label });
	Branch& branch = SubBranches.at(SubBranches.size() - 1);
	branch.Data = data;
	return branch;
}

Branch::string Branch::ToString() const
{
	std::stringstream ss;
	for (const auto& branch : SubBranches)
	{
		ss << branch.ThisString(0) << '\n';
	}
	return ss.str();
}

Branch::operator string() const
{
	return ToString();
}

Branch::string Branch::ThisString(unsigned int tabs) const
{
	std::stringstream ss;
	for (unsigned int i = 0; i < tabs; i++)
		ss << "  ";
	if (tabs >= 0)
		ss << "\\__\n";
	for (unsigned int i = 0; i < tabs; i++)
		ss << "  ";
	ss << "    " << Label;
	if (Data.has_value())
	{
		ss << ": ";
		try
		{
			ss << std::any_cast<byte>(Data);
		}
		catch (const std::bad_any_cast&)
		{
			try
			{
				ss << std::any_cast<size>(Data);
			}
			catch (const std::bad_any_cast&)
			{
				try
				{
					ss << std::any_cast<integer>(Data);
				}
				catch (const std::bad_any_cast&)
				{
					try
					{
						ss << std::any_cast<string>(Data);
					}
					catch (const std::bad_any_cast&)
					{
						try
						{
							ss << std::any_cast<string>(Data);
						}
						catch (const std::bad_any_cast&)
						{
							try
							{
								ss << std::any_cast<string_view>(Data);
							}
							catch (const std::bad_any_cast&)
							{
								try
								{
									ss << std::any_cast<cstring>(Data);
								}
								catch (const std::bad_any_cast&)
								{
									ss << Data.type().name();
								}
							}
						}
					}
				}
			}
		}
	}
	else
	{
		ss << '\n';
		for (const auto& branch : SubBranches)
		{
			ss << branch.ThisString(tabs + 1) << '\n';
		}
	}

	return ss.str();
}