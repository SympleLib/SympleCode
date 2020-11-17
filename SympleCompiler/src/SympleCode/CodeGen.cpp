#include "SympleCode/CodeGen.hpp"

#include <sstream>

namespace Symple::CodeGen
{
	std::string Gen(const Tree& tree)
	{
		std::stringstream ss;
		for (size_t i = 0; i < tree.SubBranches.size(); i++)
		{
			const Branch& branch = tree.SubBranches[i];
			if (branch.Label == "Var")
			{
				ss << std::any_cast<std::string>(branch.FindBranch("Type").Data) << std::any_cast<std::string>(branch.FindBranch("Name").Data);
			}
			else if (branch.Label == "Add")
			{
				
			}
		}

		return ss.str();
	}
}