#pragma once

#include <string>
#include <sstream>

#include "SympleCode/Tree/Tree.hpp"

namespace Symple::CodeGenerator
{
	void GenerateCode(const std::string& in, const std::string& out);
	void HandleBranch(const Branch& branch);
}