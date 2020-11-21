#include "SympleCode/Tree/Tree.hpp"

#include <sstream>

#include "SympleCode/Util/Util.hpp"
#include "SympleCode/Type.hpp"
#include "SympleCode/Token.hpp"

namespace Symple
{
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

	void Branch::PopBranch()
	{
		if (SubBranches.size() <= 0)
		{
			Err("Branch has no elements!");
			abort();
		}
		SubBranches.erase(SubBranches.end() - 1);
	}

	void Branch::PopBranch(size_t index)
	{
		SubBranches.erase(SubBranches.begin() + index);
	}

	void Branch::PopBranch(const std::string& label)
	{
		PopBranch(FindBranchIndex(label));
	}

	Branch& Branch::FindBranch(const std::string& label)
	{
		for (auto& Branch : SubBranches)
			if (Branch.Label == label)
				return Branch;
		Err("Branch '%s' Doesn't Exist!", label.c_str());
		abort();
	}

	const Branch& Branch::FindBranch(const std::string& label) const
	{
		for (const auto& Branch : SubBranches)
			if (Branch.Label == label)
				return Branch;
		Err("Branch '%s' Doesn't Exist!", label.c_str());
		abort();
	}

	size_t Branch::FindBranchIndex(const std::string& label) const
	{
		for (size_t i = 0; i < SubBranches.size(); i++)
			if (SubBranches[i].Label == label)
				return i;
		Err("Branch '%s' Doesn't Exist!", label.c_str());
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

	bool Branch::operator ==(const Branch& other) const
	{
		return Label == other.Label && (Data.has_value() || SubBranches == other.SubBranches);
	}

	bool Branch::operator !=(const Branch& other) const
	{
		return !(*this == other);
	}

	Branch::string Branch::ThisString(std::string indent, bool last) const
	{
		std::stringstream ss;
		ss << '\n' << indent;
		if (last)
			ss << "L--\t";
		else
			ss << "|--\t";
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
														Branch branch = std::any_cast<Branch>(Data);
														if (last)
															indent += " \t";
														else
															indent += "|\t";
														ss << branch.ThisString(indent);
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
				indent += " \t";
			else
				indent += "|\t";
			for (size_t i = 0; i < SubBranches.size(); i++)
			{
				ss << SubBranches[i].ThisString(indent, i == SubBranches.size() - 1);
			}
		}

		return ss.str();
	}
}