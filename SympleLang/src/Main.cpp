#include <pch.h>

#include <cstdlib>
#include <cstdio>

#include "SympleCode/Util/Log.h"

int main()
{
	using namespace Symple;

	SympleLog->GetLogger()->critical("Just Kidden");

	system("pause");
}