#pragma once

#include <vector>
#include <string>

#define Out(...) fprintf(stdout, __VA_ARGS__)
#define Err(...) fprintf(stderr, __VA_ARGS__)

#define COut(msg) std::cout << msg;
#define CErr(msg) std::cerr << msg;

namespace Symple
{
	std::vector<std::string> Split(std::string string, const std::string& delimiter);
	void Write(const char* path, const char* data);

	int powi(int v, int t);
}