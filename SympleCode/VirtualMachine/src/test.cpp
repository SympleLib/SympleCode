#include "svar_system.h"

#include <iostream>

int main()
{
	svarid myVar = svm::CreateIntVarieble("myVar", 69420);
	printf("var myVar = %i;\n", svm::GetIntValue(myVar));
	svm::SetIntValue(myVar, 1337);
	printf("myVar = %i;\n", svm::GetIntValue(myVar));
	svm::DeleteVarieble("myVar");
	printf("delete myVar;\n");

	system("pause");
}