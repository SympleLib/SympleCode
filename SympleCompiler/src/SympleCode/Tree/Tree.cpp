#include "SympleCode/Tree/Tree.hpp"

#include <sstream>

#include "SympleCode/Util/Type.hpp"
#include "SympleCode/Token.hpp"

Branch::Branch() {}

Branch::Branch(const std::string& label)
	: Label(label) {}

Branch::Branch(const std::string& label, const std::any& data)
	: Label(label), Data(data) {}

Branch& Branch::PushBranch(const Branch& branch)
{
	SubBranches.push_back({ branch });
	return SubBranches.at(SubBranches.size() - 1);
}

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

Branch& Branch::FindBranch(const std::string& label)
{
	for (auto& Branch : SubBranches)
		if (Branch.Label == label)
			return Branch;
	fprintf(stderr, "Branch '%s' Doesn't Exist!", label.c_str());
	abort();
}

const Branch& Branch::FindBranch(const std::string& label) const const
{
	for (const auto& Branch : SubBranches)
		if (Branch.Label == label)
			return Branch;
	fprintf(stderr, "Branch '%s' Doesn't Exist!", label.c_str());
	abort();
}

Branch::string Branch::ToString() const
{
	if (SubBranches.size() > 0)
	{
		std::stringstream ss;
		for (size_t i = 0; i < SubBranches.size(); i++)
		{
			ss << SubBranches[i].ThisString({}, i == SubBranches.size() - 1);
		}
		return ss.str().substr(1);
	}
	return {};
}

Branch::operator string() const
{
	return ToString();
}

Branch::string Branch::ThisString(std::string indent, bool last) const
{
	std::stringstream ss;
	ss << '\n' << indent;
	if (last)
		ss << "L-- ";
	else
		ss << "|-- ";
	ss << Label;
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
						ss << std::any_cast<int>(Data);
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
										try
										{
											ss << Tokens::ToString(std::any_cast<TokenInfo>(Data).GetToken()) << " [" << std::any_cast<TokenInfo>(Data).GetLex() << ']';
										}
										catch (const std::bad_any_cast&)
										{
											try
											{
												ss << Tokens::ToString(std::any_cast<Token>(Data));
											}
											catch (const std::bad_any_cast&)
											{
												try
												{
													if (last)
														indent += "   ";
													else
														indent += "|  ";
													ss << std::any_cast<Branch>(Data).ThisString(indent);
												}
												catch (const std::bad_any_cast&)
												{
													try
													{
														ss << std::any_cast<Type>(Data).Name << " (" << std::any_cast<Type>(Data).Size << " bytes)";
													}
													catch (const std::bad_any_cast&)
													{
													}
												}
											}
										}
									}
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
		if (last)
			indent += "   ";
		else
			indent += "|  ";
		for (size_t i = 0; i < SubBranches.size(); i++)
		{
			ss << SubBranches[i].ThisString(indent, i == SubBranches.size() - 1);
		}
	}

	return ss.str();
}