#include <iostream>

#include <scm.h>
#include <scp.h>
#include <svm.h>

int main()
{
	scm::InitPrimitiveClasses();
	scp::ParseSource(R"(
int myInt;
	)");
	
	printf("myInt - Class: %u, Type: %u, Value: %i;", scm::GetClassID("int"), scm::GetClassType(scm::GetClassID("int")), svm::GetIntValue(svm::GetVariebleID("myInt")));

	system("pause");
}