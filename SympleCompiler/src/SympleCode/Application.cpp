#include <windows.h>
#include <iostream>

#include "SympleCode/Parser/Parser.h"
#include "SympleCode/Common/Node/BinaryExpressionNode.h"

int main()
{
	SetConsoleTitle(L"Symple Compiler - Treidex");

	Symple::Parser parser("5 / 1 + 2 * 3");
	Symple::ExpressionNode* tree = parser.ParseBinaryExpression();
	std::cout << tree->ToString();

	std::cin.get();
}