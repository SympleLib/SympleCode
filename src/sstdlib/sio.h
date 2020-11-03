#ifndef _SSTDB_LIB_SIO_H_
#define _SSTDB_LIB_SIO_H_
#include "../svm.h"

#include <vector>

namespace sstdlib
{
	void Print(const std::vector<sstring>&);
	void Read(sid);

	std::string ReadFile(const std::string&);
}
#endif