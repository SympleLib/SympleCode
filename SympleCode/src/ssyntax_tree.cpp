#include "ssyntax_tree.h"

#include <unordered_map>

#define EXPRESION -1

namespace symple
{
	std::unordered_map<size_t, std::vector<size_t>> tree;
	std::vector<std::string> tokens;

	void InitializeTokens()
	{
		tokens.push_back("expre"); // Expresion

		tokens.push_back("if");    // If Condition
		tokens.push_back("else");  // Else Condition
		tokens.push_back("while"); // While Conditional Loop

		tokens.push_back("("); // Begin Section
		tokens.push_back(")"); // End Section
		tokens.push_back("["); // Begin Index
		tokens.push_back("]"); // End Index
		tokens.push_back("{"); // Begin Body
		tokens.push_back("}"); // End Body

		tokens.push_back("->"); // Acces Function
		tokens.push_back(".");  // Access Field

		tokens.push_back("++"); // Unary operater
		tokens.push_back("--"); // Unary operater
		tokens.push_back("+");  // Addition
		tokens.push_back("-");  // Subtraction
		tokens.push_back("*");  // Multiplication
		tokens.push_back("/");  // Division

		tokens.push_back("@"); // Get Pointer
		tokens.push_back("$"); // Dereference Pointer

		tokens.push_back("|"); // OR
		tokens.push_back("&"); // AND
		tokens.push_back("^"); // XOR
		tokens.push_back("~"); // Reverse Bits

		tokens.push_back("||"); // Logical OR
		tokens.push_back("&&"); // Logical AND
		tokens.push_back("^^"); // Logical XOR
		tokens.push_back("!");  // Logical Reverse Bits
		tokens.push_back("=="); // Logical Equals
		tokens.push_back("!="); // Logical Not Equals
		
		tokens.push_back(";"); // End of line

		tokens.push_back("class");     // Full Fleshed has fields and functions
		tokens.push_back("struct");    // Only has fields
		tokens.push_back("interface"); // Only Functions

		tokens.push_back("var"); // Create Varieble
	}

	void InitializeTree()
	{
		// If Tree
		tree[0] = std::vector<size_t>();
		tree[0].push_back(GetToken("("));
		tree[0].push_back(EXPRESION);
		tree[0].push_back(GetToken(")"));
		tree[0].push_back(GetToken("{"));
		tree[0].push_back(EXPRESION);
		tree[0].push_back(GetToken("}"));
	}

	size_t GetToken(const std::string& str)
	{
		return std::find(tokens.begin(), tokens.end(), str) - tokens.begin();
	}
}