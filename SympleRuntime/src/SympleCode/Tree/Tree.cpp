#include "SympleCode/Tree/Tree.hpp"

#include <sstream>

Branch& Branch::PushBranch(const std::string& label)
{
	SubBranches.insert({ label, { label } });
	return SubBranches.at(label);
}

Branch::string Branch::ToString() const
{
	return ThisString(0);
}

Branch::operator string() const
{
	return ToString();
}

Branch::string Branch::ThisString(unsigned int tabs) const
{
	std::stringstream ss;
	for (unsigned int i = 0; i < tabs; i++)
		ss << '\t';
	ss << Label << ": ";
	if (Data.has_value())
	{
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
						ss << Data.type().name();
					}
				}
			}
		}
	}
	else
	{
		ss << '\n';
		for (const auto& items : SubBranches)
		{
			ss << items.second.ThisString(tabs + 1) << '\n';
		}
	}

	return ss.str();
}